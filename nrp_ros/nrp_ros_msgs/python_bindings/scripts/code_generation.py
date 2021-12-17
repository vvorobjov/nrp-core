# NRP Core - Backend infrastructure to synchronize simulations
#
# Copyright 2020-2021 NRP Team
#
# Licensed under the Apache License, Version 2.0 (the "License");
# you may not use this file except in compliance with the License.
# You may obtain a copy of the License at
#
#   http://www.apache.org/licenses/LICENSE-2.0
#
# Unless required by applicable law or agreed to in writing, software
# distributed under the License is distributed on an "AS IS" BASIS,
# WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
# See the License for the specific language governing permissions and
# limitations under the License.
#
# This project has received funding from the European Union’s Horizon 2020
# Framework Programme for Research and Innovation under the Specific Grant
# Agreement No. 945539 (Human Brain Project SGA3).
#
# This file is a modified version of another one originally authored by:
# Bhaskara Marthi with license BSD.
# The original file can be found at: https://github.com/bhaskara/boost_python_ros

from io import StringIO
from roslib import packages
from roslib import msgs
import rospkg
import os
import re
import itertools as it

############################################################
# Main functions
############################################################


def generate_export_function(spec, s):
    "Generate the single helper function that exports the definition of a message type."

    msg = spec.short_name
    s.write("void export{0} ()".format(msg))
    s.write("{")
    with Indent(s, 2):
        s.write("using {0}::{1};".format(spec.package, msg))
        s.write('DataPack<{0}>::create_python("{0}DataPack");'.format(msg))
        s.write('class_<{0}, {0}*, shared_ptr<{0}> >("{0}", "{1}")'.format(msg, class_docstring(spec)))

        array_fields = []

        with Indent(s, 2):
            for field in spec.parsed_fields():
                s.write('.def_readwrite("{0}", &{1}::{0})'.format(field.name, msg))
                if field.is_array:
                    array_fields.append(field)
            s.write(';\n')

        s.write("register_ptr_to_python<const {0} *>();\n".format(msg))

        for f in array_fields:
            s.write('class_<vector<{0}> >("{1}_{2}")'.format(qualify(f.base_type),
                                                               msg, f.name))
            with Indent(s, 2):
                s.write('.def(vector_indexing_suite<vector<{0}> > ())'.format(qualify(f.base_type)))
                s.write(';')
    s.write("}\n")


def generate_equality_forward_declarations(spec, s):
    already_seen = set()
    for f in spec.parsed_fields():
        if f.is_array and not f.is_builtin:
            if f.base_type in already_seen:
                continue
            already_seen.add(f.base_type)
            m = re.match('(\w+)/', f.base_type)
            pkg = m.group(1)
            s.write("namespace {0}".format(pkg))
            s.write("\n{")
            s.write("inline bool operator== (const {0}& /*m1*/, const {0}& /*m2*/)".
                    format(qualify(f.base_type)))
            s.write("{\n  return false;\n}\n\n} // namespace")
            #s.write("namespace {0}".format(pkg))
            #s.write("{\n")
            #s.write("bool operator== (const {0}& m1, const {0}& m2);\n".\
            #        format(qualify(f.base_type)))
            #s.write("}\n")

def generate_file(pkg, msg, s=None):
    "Generate the definition file for a single message"
    
    if s is None:
        s = IndentedWriter()
    spec = get_msg_spec(pkg, msg)

    s.write("#include <{0}/{1}.h>".format(pkg, msg))
    s.write("#include <boost/python.hpp>")
    s.write("#include <boost/python/suite/indexing/vector_indexing_suite.hpp>")
    s.write("#include \"nrp_general_library/datapack_interface/datapack.h\"\n")
    s.write("\n")

    generate_equality_forward_declarations(spec, s)

    s.write("namespace {0}".format(pkg))
    s.write("{")
    s.write("\n")

    s.write("using namespace boost::python;")
    s.write("using boost::shared_ptr;")
    s.write("using std::vector;")
    s.write("\n")
    
    s.write("// Dummy equality check to avoid compilation error for vector_indexing_suite")
    s.write("bool operator== (const {0}& /*m1*/, const {0}& /*m2*/)".format(msg))
    s.write("{")
    with Indent(s, 2):
        s.write("return false;")
    s.write("}\n")

    generate_export_function(spec, s)

    s.write("} // namespace")

    return s.getvalue()


def generate_package_file(pkg, s=None):
    "Generate the top level file for the given package with forward declarations for individual messages"
    if s is None:
        s = IndentedWriter()

    # Header
    s.write("#include <boost/python.hpp>\n")
    s.write("namespace {0}".format(pkg))
    s.write("{\n")

    # Forward declarations
    messages = msgs.list_msg_types(pkg, False)
    for m in messages:
        s.write("void export{0}();".format(m));

    # Boost python wrappers
    s.write("\n")
    s.write("BOOST_PYTHON_MODULE({0})".format(pkg))
    s.write("{")
    with Indent(s, 2):
        for m in messages:
            s.write("export{0}();".format(m))
    s.write("}\n")

    s.write("} // namespace")
    return s.getvalue()


def generate_rospy_conversion(pkg, msg, s=None):
    if s is None:
        s = IndentedWriter()

    spec = get_msg_spec(pkg, msg)
    s.write("def {0}_to_ros(x):".format(msg))
    with Indent(s, 4):
        s.write("try:")
        with Indent(s, 4):
            s.write("m = {0}.msg.{1}()".format(pkg, msg))
            for f in spec.parsed_fields():
                def field_conversion(e):
                    f_conv = '{0}_to_ros'.format(f.base_type[f.base_type.find("/")+1:])
                    return e if f.is_builtin else '{0}({1})'.format(f_conv, e)

                if f.type in ['time', 'duration'] and f.is_array:
                    s.write("Conversion of time or duration array fields is not supported. Field {0} won't be converted"
                            .format(f.name))
                elif f.type in ['time', 'duration']:
                    s.write("m.{0}.secs = x.{0}.secs".format(f.name))
                    s.write("m.{0}.nsecs = x.{0}.nsecs".format(f.name))
                elif f.is_array:
                    s.write("l = []".format(f.name))
                    s.write("for e in x.{0}:".format(f.name))
                    with Indent(s, 4):
                        s.write("l.append({0})".format(field_conversion('e')))

                    if f.base_type == "uint8":
                        s.write("l = bytes(l)")

                    s.write("m.{0} = l".format(f.name))
                else:
                    e = "x.{0}".format(f.name)
                    s.write("m.{0} = {1}".format(f.name, field_conversion(e)))

            s.write("return m")

        s.write("except Exception as e:")
        with Indent(s, 4):
            s.write("if not isinstance(x, cpp.{0}):".format(msg))
            with Indent(s, 4):
                error_msg = "\"'{0}_to_ros' must be called with objects of type 'nrp_core.data.nrp_ros.{1}.{0}', " \
                            "called with object of type '{{}}'\"".format(msg, pkg)
                s.write("raise TypeError({}.format(type(x)))".format(error_msg))
            s.write("else:")
            with Indent(s, 4):
                s.write("raise e\n")

    s.write("def {0}_to_boost(x):".format(msg))
    with Indent(s, 4):
        s.write("try:")
        with Indent(s, 4):
            s.write("m = cpp.{0}()".format(msg))
            for f in spec.parsed_fields():
                def field_conversion(e):
                    f_conv = '{0}_to_boost'.format(f.base_type[f.base_type.find("/")+1:])
                    return e if f.is_builtin else '{0}({1})'.format(f_conv, e)

                if f.type in ['time', 'duration'] and f.is_array:
                    s.write("Conversion of time or duration array fields is not supported. Field {0} won't be converted"
                            .format(f.name))
                elif f.type in ['time', 'duration']:
                    s.write("m.{0}.secs = x.{0}.secs".format(f.name))
                    s.write("m.{0}.nsecs = x.{0}.nsecs".format(f.name))
                elif f.is_array:
                    s.write("for e in x.{0}:".format(f.name))
                    with Indent(s, 4):
                        s.write("m.{0}.append({1})".format(f.name, field_conversion('e')))
                else:
                    e = "x.{0}".format(f.name)
                    s.write("m.{0} = {1}".format(f.name, field_conversion(e)))

            s.write("return m")

        s.write("except Exception as e:")
        with Indent(s, 4):
            s.write("if not isinstance(x, cpp.{0}):".format(msg))
            with Indent(s, 4):
                # TODO: fix msg
                error_msg = "\"'{0}_to_boost' must be called with objects of type '{1}.msg.{0}', " \
                            "called with object of type '{{}}'\"".format(msg, pkg)
                s.write("raise TypeError({}.format(type(x)))".format(error_msg))
            s.write("else:")
            with Indent(s, 4):
                s.write("raise e\n")
    
    return s.getvalue()


def generate_python_imports(pkg):
    pkgs = set()
    for m in msgs.list_msg_types(pkg, False):
        spec = get_msg_spec(pkg, m)
        for f in spec.parsed_fields():
            if f.is_header:
                pkgs.add('std_msgs')
            else:
                match = re.match('(\w+)/', f.base_type)
                if match and match.group(1)!=pkg:
                    pkgs.add(match.group(1))
    return '\n'.join("from nrp_core.data.nrp_ros.{0}_conversions import *".format(p) for p in pkgs)
    

def write_rospy_conversions(pkg, target_dir, current_pkg):
    """
    Generate all rospy conversions.
    @param pkg: We'll generate conversion functions for all messages defined in this ros package.
    @param current_pkg: The generated functions are being put into this ros package.
    """
    outfile = os.path.join(target_dir, pkg+'_conversions.py')
    with open(outfile, 'w') as f:
        f.write("import nrp_core.data.nrp_ros.rostime_boost_python\n")
        f.write(generate_python_imports(pkg))
        f.write("\nimport {0}.msg\n".format(pkg))
        f.write("import nrp_core.data.nrp_ros.{0} as cpp\n".format(pkg))


        for m in msgs.list_msg_types(pkg, False):
            f.write(generate_rospy_conversion(pkg, m))
        

def write_bindings(pkg, target_dir):
    "Generate and write all bindings"
    #print(pkg)
    #print(target_dir)
    top_level_file = os.path.join(target_dir, pkg+'.cpp')
    with open(top_level_file, 'w') as f:
        f.write(generate_package_file(pkg))
    for m in msgs.list_msg_types(pkg, False):
        outfile = os.path.join(target_dir, "{0}_{1}.cpp".format(pkg, m))
        with open(outfile, 'w') as f:
            f.write(generate_file(pkg, m))
                  
    
    
    
                
############################################################
# Helpers
############################################################

MSG_TYPE_TO_CPP = {'byte': 'int8_t', 'char': 'uint8_t',
                   'bool': 'uint8_t',
                   'uint8': 'uint8_t', 'int8': 'int8_t', 
                   'uint16': 'uint16_t', 'int16': 'int16_t', 
                   'uint32': 'uint32_t', 'int32': 'int32_t',
                   'uint64': 'uint64_t', 'int64': 'int64_t',
                   'float32': 'float',
                   'float64': 'double',
                   'string': 'std::string',
                   'time': 'ros::Time',
                   'duration': 'ros::Duration'}

    
def qualify(name):
    if '/' in name:
        return name.replace('/', '::')
    else:
        return MSG_TYPE_TO_CPP[name]

def get_msg_spec(pkg, msg):
    path = rospkg.RosPack().get_path(pkg)
    return msgs.load_from_file("{0}/msg/{1}.msg".format(path, msg), pkg)[1]

def class_docstring(spec):
    return spec.text.replace('"', '\\"').replace('\n', '\\n')
    

############################################################
# Indented writer
############################################################

class IndentedWriter():

    def __init__(self, s=None):
        self.str = s or StringIO()
        self.indentation = 0
        self.block_indent = False

    def write(self, s, indent=True, newline=True):
        if not indent:
            newline = False
        if self.block_indent:
            self.block_indent = False
        else:
            if newline:
                self.str.write('\n')
            if indent:
                for i in range(self.indentation):
                    self.str.write(' ')
        self.str.write(s)

    def newline(self):
        self.str.write('\n')

    def inc_indent(self, inc=2):
        self.indentation += inc

    def dec_indent(self, dec=2):
        self.indentation -= dec

    def reset_indent(self):
        self.indentation = 0

    def block_next_indent(self):
        self.block_indent = True

    def getvalue(self):
        return self.str.getvalue()

class Indent():

    def __init__(self, w, inc=2, indent_first=True):
        self.writer = w
        self.inc = inc
        self.indent_first = indent_first

    def __enter__(self):
        self.writer.inc_indent(self.inc)
        if not self.indent_first:
            self.writer.block_next_indent()

    def __exit__(self, type, val, traceback):
        self.writer.dec_indent(self.inc)



