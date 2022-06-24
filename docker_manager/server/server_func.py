import os
import shutil
import zipfile

import docker
import time

def unzip_file(zip_file_name):
    store_path = os.path.dirname(zip_file_name)
    with zipfile.ZipFile(zip_file_name, "r") as zip_file:
        for f_name in zip_file.namelist():
            zip_file.extract(f_name, store_path)

    os.remove(zip_file_name)
    return store_path


def remove_folders(folders):
    for f in folders:
        shutil.rmtree(f)


class DockerContainer(object):
    def __init__(self, share_path, prefix_name):
        super().__init__()
        self.container_name_prefix = prefix_name
        self.client = docker.from_env()
        self.share_volume = os.path.join('/share', share_path)
        cur_share_path = os.path.join(os.getcwd(), share_path)

        self.the_host_config = self.client.api.create_host_config(binds={
                cur_share_path: {
                    'bind': self.share_volume,
                    'mode': 'rw',
                }
            },
            network_mode="host"
        )

    def run_container(self, exec_info):
        """
        Create a necessary docker container
        """
        create_res = self.create_container(exec_info["img"])
        if create_res["Error"] != "":
            return create_res
        """
        Launch a simulator engine inside the container
        """
        exec_res = self.docker_execution(create_res["id"], exec_info)

        return exec_res

    def inspect_container_id(self, container_id):
        """ Check the status of the container """
        try:
            self.client.api.inspect_container(container_id)
            return "RUNNING"  # The container is existing
        except OSError as err:
            return "STOPPED"  # The container is removed

    def remove_container(self, container_id):
        """ Kill and remove the created container """
        try:
            self.client.api.stop(container_id)
            res = self.client.api.wait(container_id)
            self.client.api.remove_container(container_id,
                v=True, force=True)
        except:
            return {"Error":'Not Found'}
        if res["Error"] is None:
            res["Error"] = ""
        return res

    def create_container(self, image_name):
        """
        Create a necessary docker container
        """
        msg = ""
        container_id = -1
        try:
            res = self.client.api.create_container(
                image=image_name, tty=True,detach=True, 
                volumes=[self.share_volume], host_config=self.the_host_config)
            self.client.api.start(res)
            container_id = res['Id']
            container_short_id =  container_id[:12]
            container_name = self.container_name_prefix + container_short_id
            self.client.api.rename(container_id, container_name)
        except OSError as err:
            msg = "{0}".format(err)
        except BaseException as err:
            msg = f"{err=}, {type(err)=}"
        return {"id": container_id, "Error":msg}

    def docker_execution(self, container_id, exec_info):
        """
        Launch a simulator engine inside the container
        """
        exec_cmd = exec_info["ExecCmd"]
        exec_env = exec_info["ExecEnvironment"]
        exec_dir = self.share_volume

        msg = ""
        try:
            resp = self.client.api.exec_create(
            container_id, exec_cmd, tty=True,
            environment=exec_env, workdir=exec_dir)
            exec_output = self.client.api.exec_start(
                resp['Id'], tty=True, stream=True)
            exit_code = self.client.api.exec_inspect(resp['Id'])['ExitCode']
            if exit_code is not None:
                msg = f"Docker execution fail with error code {exit_code} --> "
                msg = msg + str(next(exec_output))
                self.remove_container(container_id)
        except OSError as err:
            msg = "{0}".format(err)
            self.remove_container(container_id)
        except BaseException as err:
            msg = f"{err=}, {type(err)=}"
            self.remove_container(container_id)

        return {"id": container_id, "Error":msg}

    def clear_nrp_containers(self):
        containers_name = []
        containers = self.client.containers.list(
            all=True, filters={'name':self.container_name_prefix})
        for container in containers:
            containers_name.append(container.name)
            self.remove_container(container.id)
        return containers_name