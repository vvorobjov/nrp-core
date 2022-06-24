import argparse
import logging

from flask import Flask, request, send_from_directory
from server_func import *
import os,signal,time

log = logging.getLogger('werkzeug')
log.setLevel(logging.ERROR)

app = Flask("NRP")
app.config['UPLOAD_FOLDER'] = 'experiment_data'
app.config["CONTAINER_NAME_PREFIX"] = 'nrp_'
app.config['DOCKER_CLIENT'] = None

# ------------------------------------------------------------ #
# ----------------- DockerLauncher API ----------------------- #
# ------------------------------------------------------------ #
@app.route('/uploader', methods=['POST'])
def uploader():
    the_file = request.files['file']
    upload_path = app.config['UPLOAD_FOLDER'] + '/'
    the_file.save(os.path.join(upload_path, the_file.filename))
    file_name = upload_path + the_file.filename

    err = ""
    store_path = "errors"
    try:
        store_path = unzip_file(file_name)
    except ValueError as e:
        err = "Unzip file error: " + e.__str__()
    except BaseException as e:
        err = "Unzip file error: " + e.__str__()
    msg = "Unzip file <{0}> to path <{1}>".format(file_name, store_path)
    app.logger.info(msg)

    return {"msg":msg, "Error": err}


@app.route('/downloader', methods=['GET'])
def downloader():
    file_name = request.json["filename"]
    return send_from_directory(
        app.config['UPLOAD_FOLDER'], file_name, as_attachment=True
    )


@app.route("/clear", methods=['GET'])
def clear():
    app.logger.info('Shutting down...')
    rmFiles = [app.config['UPLOAD_FOLDER']]
    remove_folders(rmFiles)
    remove_containers = app.config['DOCKER_CLIENT'].clear_nrp_containers()
    app.logger.info(remove_containers)
    return {"msg":"Cleared"}


@app.route("/force_shutdown", methods=['GET'])
def force_shutdown():
    os.kill(os.getpid(), signal.SIGINT)
    return {"msg":"Force shutting down..."}


# ------------------------------------------------------------ #
# --------------- Docker containers management API ----------- #
# ------------------------------------------------------------ #
@app.route('/docker_run', methods=['POST'])
def docker_exec():
    exec_info = request.json
    res = app.config['DOCKER_CLIENT'].run_container(exec_info)
    app.logger.info(res)
    return res


@app.route('/docker_stop', methods=['POST'])
def docker_stop():
    container_id = request.json["id"]
    res = app.config['DOCKER_CLIENT'].remove_container(container_id)
    app.logger.info(res)
    return res


@app.route('/docker_status', methods=['POST'])
def docker_status():
    container_id = request.json["id"]
    container_status = app.config['DOCKER_CLIENT'].inspect_container_id(container_id)

    return {"status": container_status, "Error": ""}


if __name__ == '__main__':
    parser = argparse.ArgumentParser("Description.")
    parser.add_argument('--port',
                        type=int,
                        default=5002,
                        help="The port on which the DockerLauncher will wait for requests")
    args = parser.parse_args()
    server_port = args.port

    if not os.path.exists(app.config['UPLOAD_FOLDER']):
        os.makedirs(app.config['UPLOAD_FOLDER'])
    app.debug = True
    app.config['DOCKER_CLIENT'] = DockerContainer(
        app.config['UPLOAD_FOLDER']+'/', app.config["CONTAINER_NAME_PREFIX"])
    try:
        app.run(host='0.0.0.0', port=server_port)
    except RuntimeError as msg:
        print("Execution error: ", msg)
