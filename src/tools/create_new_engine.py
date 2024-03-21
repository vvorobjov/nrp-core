#!/usr/bin/env python3

from string import Template
from pathlib import Path
import os
import argparse


def change_cwd() -> None:
    """Change working directory to the script's directory"""
    abspath = os.path.abspath(__file__)
    dname = os.path.dirname(abspath)
    os.chdir(dname)


def parse_arguments():
    """Parses command line arguments"""
    parser = argparse.ArgumentParser()
    parser.add_argument("-n", "--name", help="Base name of the new engine", required=True)
    parser.add_argument("-t", "--type", choices=["grpc", "json"], help="Type of the new engine", required=True)
    return parser.parse_args()


def get_template_root(args) -> str:
    """Returns root directory of the chosen template engine"""
    template_root_dirs = dict(
        json = "template_json_engine/",
        grpc = "template_grpc_engine/"
    )

    return template_root_dirs[args.type]


def fill_template(filename: str, substitutions: dict) -> str:
    """Fills the template file using given substitution strings"""
    with open(filename, 'r') as file:
        template = Template(file.read())
        filled_template = template.substitute(substitutions)

        return filled_template


def save_filled_template(filename: str, filled_template: str) -> None:
    """Saves the filled template as file with given filename"""
    with open(filename, 'w') as file:
        file.write(filled_template)


def create_directory(old_dir_path: str, substitutions: dict) -> None:
    """Creates new directory corresponding to the directory in the template engine"""
    new_dir_path = old_dir_path.replace("template", substitutions["engine_name_lowercase"]) + "/"
    Path(new_dir_path).mkdir(parents=True, exist_ok=True)


def generate_files(old_dir_path: str, filenames: list, substitutions: dict) -> None:
    """Fills every template file on the given list of files and saves them in the new directory"""
    for filename in filenames:
        old_filename = old_dir_path + "/" + filename
        new_filename = (old_dir_path + "/" + filename).replace("template", substitutions["engine_name_lowercase"])

        filled_template = fill_template(old_filename, substitutions)
        save_filled_template(new_filename, filled_template)


# Parse command line arguments

args = parse_arguments()

change_cwd()

# Create a dictionary with (placeholder - substitution string) pairs
# It will be used to fill the template source files and directories

substitutions = dict(
    engine_name           = args.name,
    engine_name_lowercase = args.name.lower(),
    engine_name_uppercase = args.name.upper()
)

template_root = get_template_root(args)

# Check if the directory exists

if(not Path(template_root).is_dir()):
    print("Template engine root directory '" + template_root + "' doesn't exist")
    exit(1)

# Walk over directories of the template engine
# Fill every template file and create a new engine with the same
# directory structure as the template engine

for (dirpath, dirnames, filenames) in os.walk(template_root):
    create_directory(dirpath, substitutions)
    generate_files(dirpath, filenames, substitutions)

# EOF
