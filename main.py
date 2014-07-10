#!/usr/bin/python
import os
import hashlib
from flask import Flask, request, redirect, url_for
from werkzeug import secure_filename

UPLOAD_FOLDER = './static'
ALLOWED_EXTENSIONS = set(['png', 'jpg', 'jpeg', 'gif'])

app = Flask(__name__)
app.config['UPLOAD_FOLDER'] = UPLOAD_FOLDER
app.config['MAX_CONTENT_LENGTH'] = 16 * 1024 * 1024

def allowed_file(filename):
    return '.' in filename and \
           filename.rsplit('.', 1)[1] in ALLOWED_EXTENSIONS

@app.route('/', methods=['GET', 'POST'])
def upload_file():
    if request.method == 'POST':
        file = request.files['file']
        if file and allowed_file(file.filename):
            _, ext = os.path.splitext(file.filename)
            m = hashlib.sha256()
            m.update(file.read())
            print("files:",file.read())
            filename = m.hexdigest() + ext
            filename = secure_filename(filename)
            file.save(os.path.join(app.config['UPLOAD_FOLDER'], filename))
            return redirect(url_for('recognize', filename=filename))
    return '''
    <!doctype html>
    <title>Upload new File</title>
    <h1>Upload new File</h1>
    <form action="" method="post" enctype="multipart/form-data">
      <p><input type="file" name="file" id="file" / >
         <input type=submit value=Upload />
         </p>
    </form>
    '''

@app.route('/recognize/<filename>', methods=['GET'])
def recognize(filename = ""): 
    url = url_for('static', filename=filename)
    return '<html><body><img src="' + url + '" /></body></html>'


if __name__ == '__main__':
    app.debug = True
    app.run('0.0.0.0', port = 80)

