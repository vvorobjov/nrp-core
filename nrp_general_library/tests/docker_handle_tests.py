from nrp_core.docker_handle import NRPDockerHandle

import unittest

from time import sleep


class TestNrpHandle(unittest.TestCase):

    def test_basic(self):

        nrp_docker = NRPDockerHandle('unix:///var/run/docker.sock',
                                     'nrp-core/nrp-gazebo-nest-ubuntu20:latest',
                                     'docker_handle_test_folder',
                                     '/bin/bash -c "echo \"hi\" && echo $MESSAGE >| test_file2.txt && sleep 100"',
                                     ['MESSAGE=message_from_outside'])

        # Let the command reach sleep 100 in the container
        sleep(1)

        # get_status
        status = nrp_docker.get_status()

        self.assertEqual(type(status), dict)
        self.assertTrue('running' in status.keys())
        self.assertEqual(status['running'], True)
        self.assertTrue('exit_code' in status.keys())
        self.assertEqual(status['exit_code'], None)

        # get_experiment_archive
        self.assertTrue(nrp_docker.get_experiment_archive('docker_handle_test_file.txt'))
        self.assertFalse(nrp_docker.get_experiment_archive('wrong_file.txt'))
        self.assertTrue(nrp_docker.get_experiment_archive('test_file2.txt'))
        with open('docker_handle_test_folder/test_file2.txt', 'r') as f:
            self.assertEqual(f.readline(), "message_from_outside\n")

        # get_logs and stop
        logs = nrp_docker.get_logs()
        self.assertEqual(type(logs), list)
        self.assertEqual(len(logs), 0)

        self.assertEqual(nrp_docker.stop(), True)

        status = nrp_docker.get_status()
        self.assertEqual(status['running'], False)
        self.assertEqual(status['exit_code'], 137)

        logs = nrp_docker.get_logs()
        self.assertEqual(len(logs), 1)

        self.assertEqual(nrp_docker.stop(), True)

        nrp_docker.remove()

        status = nrp_docker.get_status()
        self.assertEqual(status, None)

    def test_init_exceptions(self):
        # wrong image
        with self.assertRaises(ValueError):
            NRPDockerHandle('unix:///var/run/docker.sock',
                            'wrong_image',
                            'docker_handle_test_folder',
                            '/bin/bash -c "echo \"hi\""',
                            ['MESSAGE=message_from_outside'])

        # wrong ip
        with self.assertRaises(ValueError):
            NRPDockerHandle('wrong_ip',
                            'nrp-core/nrp-gazebo-nest-ubuntu20:latest',
                            'docker_handle_test_folder',
                            '/bin/bash -c "echo \"hi\""',
                            ['MESSAGE=message_from_outside'])

        # wrong experiment folder
        with self.assertRaises(ValueError):
            NRPDockerHandle('unix:///var/run/docker.sock',
                            'nrp-core/nrp-gazebo-nest-ubuntu20:latest',
                            'wrong_folder',
                            '/bin/bash -c "echo \"hi\""',
                            ['MESSAGE=message_from_outside'])


if __name__ == '__main__':
    unittest.main()
