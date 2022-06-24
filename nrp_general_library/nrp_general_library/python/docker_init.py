import json
import os
import socket
import zipfile

import requests


class DockerConnector(object):
    """
    This class will start a docker container in a remote server,
    and receive information of the docker container
    """
    def __init__(self, configure_str): 
        super(DockerConnector, self).__init__()

        self.configure_val = json.loads(configure_str)
        self.log_info = ""
        self.container_id = -1

        self.ip_host = self.configure_val["DockerServerAddress"]
        zip_folder = self.configure_val["UploadFolder"]

        # If "UploadFolder" == "", zip the current folder
        if zip_folder == "":
            zip_folder = os.getcwd()

        zip_file = os.path.basename(zip_folder) + ".zip"

        if os.path.exists(zip_folder):
            if zip_folder[-1] != "/":
                zip_folder += "/"
            self.make_zip(zip_folder, zip_file)
            self.upload_file(zip_file)
            os.remove(zip_file)
        else:
            self.log_info = f"The folder {zip_folder} does not exist and can't be uploaded to docker server"

    """ Send a request to create a container inside the remote server """
    def initializing(self):

        config_info = {"img": self.configure_val["ImageName"],
                       "ExecCmd": f'{self.configure_val["ProcCmd"]} ' + ' '.join(self.configure_val["ProcCmdArgs"]),
                       "ExecEnvironment": self.configure_val["ExecEnvironment"]}
        try:
            # Execute program in the created docker container
            resp = requests.post("http://" + self.ip_host + \
                                 "/docker_run", json=config_info)
            cur_json = resp.json()
            self.container_id = cur_json["id"]
            self.log_info = cur_json["Error"]
        except requests.exceptions.ConnectionError as e:
            self.shutdown()
            self.log_info = "Shutdown with Connection Error" + e.__str__()
        except BaseException as e:
            self.shutdown()
            self.log_info = "Shutdown Error " + e.__str__()

    """ Shutdown the created container inside the remote server """
    def shutdown(self):
        try:
            # Stop and remove the created docker container
            resp = requests.post("http://" + self.ip_host + \
                                 "/docker_stop", json={"id": self.container_id})
            cur_json = resp.json()
            self.log_info = cur_json["Error"]
        except requests.exceptions.ConnectionError as e:
            self.log_info = "Shutdown with Connection Error" + e.__str__()
        except BaseException as e:
            self.log_info = "Shutdown Error " + e.__str__()

    """ Receive the status of the created container inside the remote server """
    def get_container_status(self):
        status = "ERROR"
        try:
            # Check the created docker container is removed or not
            resp = requests.post("http://" + self.ip_host + \
                                 "/docker_status", json={"id": self.container_id})
            cur_json = resp.json()
            status = cur_json["status"]
            self.log_info = cur_json["Error"]
        except requests.exceptions.ConnectionError as e:
            self.log_info = "Shutdown with Connection Error" + e.__str__()
        except BaseException as e:
            self.log_info = "Shutdown Error " + e.__str__()

        return status

    """ Feedback error messages to NRP client """
    def get_log_info(self):
        return self.log_info

    """ Feedback the created container ID to NRP client """
    def get_container_id(self):
        simplified_id = str(int(self.container_id, 16))
        return simplified_id[:6]

    """ Compress necessary files into ZIP """
    def make_zip(self, source_dir, output_filename):
        zipf = zipfile.ZipFile(output_filename, 'w', zipfile.ZIP_DEFLATED)
        pre_len = len(os.path.dirname(source_dir))
        for parent, dirnames, filenames in os.walk(source_dir):
            for filename in filenames:
                if filename == output_filename:
                    continue
                pathfile = os.path.join(parent, filename)
                arcname = pathfile[pre_len:].strip(os.path.sep)
                zipf.write(pathfile, arcname)
        zipf.close()

    """ Upload the necessary file to the remote server """
    def upload_file(self, file_name):
        with open(file_name, 'rb') as exp_files:
            file_info = {'file': exp_files}
            trg_url = "http://" + self.ip_host + "/uploader"
            try:
                resp = requests.post(trg_url, files=file_info)
                cur_json = resp.json()
                self.log_info = cur_json["Error"]
            except requests.exceptions.ConnectionError as e:
                self.log_info = "Shutdown with Connection Error" + e.__str__()
            except ValueError as e:
                self.log_info = "Could not convert the data: " + e.__str__()
            except BaseException as e:
                self.log_info = "Shutdown Error " + e.__str__()

    """ Download the necessary file from the remote server """
    def download_file(self, folder, filename):
        try:
            resp = requests.get("http://" + self.ip_host + \
                                "/downloader", json={"filename": filename})

            if resp.status_code == 404:
                self.log_info = f"File {filename} not found in server"
            else:
                with open(os.path.join(folder, filename), 'wb') as file:
                    file.write(resp.content)

        except requests.exceptions.ConnectionError as e:
            self.log_info = "Shutdown with Connection Error" + e.__str__()
        except BaseException as e:
            self.log_info = "Shutdown Error " + e.__str__()
