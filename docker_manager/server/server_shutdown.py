import argparse
import requests
import time

if __name__ == '__main__':
    parser = argparse.ArgumentParser(description="Shuts down the DockerLauncher at the specified address.")
    parser.add_argument('--address',
                        type=str,
                        default="0.0.0.0",
                        help="The DockerLauncher server address")
    parser.add_argument('--port',
                        type=int,
                        default=5002,
                        help="the DockerLauncher server port")
    args = parser.parse_args()

    ip_str = args.address
    port = args.port

    try:
        res_0 = requests.get(f"http://{ip_str}:{port}//clear")
        print(res_0.json())
    except requests.exceptions.ConnectionError as e:
        print(f"Shutdown with Connection Error {str(e)}")
    except BaseException as e:
        print(f"Shutdown Error {str(e)}")

    _ = requests.get(f"http://{ip_str}:{port}//force_shutdown")
    