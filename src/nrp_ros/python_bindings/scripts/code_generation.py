# NRP Core - Backend infrastructure to synchronize simulations
#
# Copyright 2020-2023 NRP Team
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
#
# ─────────────────────────────────────────────────────────────────────
# ROS 2 port notes
#
# Original ROS 1 code relied on `roslib.msgs`, `roslib.packages`, and
# `rospkg` — none of which exist in ROS 2. This rewrite:
#   - replaces those with a self-contained `.msg` parser in pure Python
#   - maps types to the ROS 2 C++ convention: pkg/Msg -> pkg::msg::Msg,
#     include path pkg/msg/msg.hpp (snake_case)
#   - treats ROS 1 primitives `time` and `duration` as the ROS 2 types
#     `builtin_interfaces/Time` and `builtin_interfaces/Duration`, which
#     use the field names `sec` / `nanosec`
# ─────────────────────────────────────────────────────────────────────

import os
import re
import itertools as it
from io import StringIO

############################################################
# Minimal .msg file parser (replaces roslib.msgs for ROS 2)
############################################################

# ROS 2 builtin_interfaces primitives — special-cased when generating
# rospy conversion helpers (their Python attribute names differ from
# ordinary message fields).
_ROS2_TIME_TYPES = frozenset({'builtin_interfaces/Time',
                              'builtin_interfaces/Duration'})

# Tokens that the .msg format treats as leaves (i.e. not user types).
_BUILTIN_PRIMITIVES = frozenset({
    'bool', 'byte', 'char',
    'uint8', 'int8', 'uint16', 'int16',
    'uint32', 'int32', 'uint64', 'int64',
    'float32', 'float64', 'string',
    'time', 'duration',                       # ROS 1 legacy, translated below
})


class FieldSpec:
    """Parsed single field from a .msg file. Interface compatible
    enough with the old roslib.msgs.Field to keep the existing
    generator code simple."""

    __slots__ = ('name', 'type', 'base_type', 'is_array', 'is_builtin', 'is_header')

    def __init__(self, name, type_str):
        self.name = name
        self.type = type_str
        self.is_array = type_str.endswith(']') or '[' in type_str
        self.base_type = type_str.split('[', 1)[0]
        self.is_builtin = ('/' not in self.base_type
                           and self.base_type in _BUILTIN_PRIMITIVES)
        # ROS 1 had a magic "Header" first field; we don't, but keep the
        # attribute so callers can assume it exists.
        self.is_header = (self.base_type == 'Header' or self.base_type == 'std_msgs/Header')


class MsgSpec:
    """Parsed .msg file. Exposes what the old roslib MsgSpec did."""

    def __init__(self, package, short_name, fields, text):
        self.package = package
        self.short_name = short_name
        self._fields = fields
        self.text = text

    def parsed_fields(self):
        return list(self._fields)


def _parse_msg_file(path, package):
    """Parse a .msg file. Ignores blank lines, '#' comments, and
    constant definitions (lines with '=').

    ROS 2 .msg files may specify default values after the field name,
    e.g. "float64 x 0.0" or 'string frame_id "world"'. We keep only
    the field name and discard the default.
    """
    fields = []
    with open(path, 'r') as f:
        text = f.read()
    for raw in text.splitlines():
        line = raw.split('#', 1)[0].strip()
        if not line or '=' in line:
            continue
        # ROS 2 field syntax:  "<type> <name> [<default>]"
        # split() on whitespace handles any number of tokens; take the
        # first as type, the second as name, ignore the rest.
        parts = line.split()
        if len(parts) < 2:
            continue
        type_str, name = parts[0], parts[1]
        fields.append(FieldSpec(name, type_str))
    short_name = os.path.splitext(os.path.basename(path))[0]
    return MsgSpec(package, short_name, fields, text)


def list_msg_types(pkg_path, include_deprecated_unused):
    """Return the list of .msg base names in the package (short names,
    e.g. ["Test"] for a file named Test.msg)."""
    msg_dir = os.path.join(pkg_path, 'msg')
    if not os.path.isdir(msg_dir):
        return []
    return sorted(os.path.splitext(f)[0]
                  for f in os.listdir(msg_dir)
                  if f.endswith('.msg'))


def get_msg_spec(pkg_path, pkg, msg):
    return _parse_msg_file(os.path.join(pkg_path, 'msg', msg + '.msg'), pkg)


############################################################
# Main functions
############################################################


def generate_export_function(spec, s):
    "Generate the single helper function that exports the definition of a message type."

    msg = spec.short_name
    s.write("void export{0} ()".format(msg))
    s.write("{")
    with Indent(s, 2):
        s.write("using {0}::msg::{1};".format(spec.package, msg))
        s.write('DataPack<{0}>::create_python("{0}DataPack");'.format(msg))
        s.write('class_<vector<const {0}*> >("{0}Vector").def(vector_indexing_suite<vector<const {0}*> >());'
                .format(msg))
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
            s.write('class_<vector<{0}> >("{1}_{2}")'.format(qualify(f.base_type, spec.package),
                                                               msg, f.name))
            with Indent(s, 2):
                s.write('.def(vector_indexing_suite<vector<{0}> > ())'.format(qualify(f.base_type, spec.package)))
                s.write(';')
    s.write("}\n")


def generate_equality_forward_declarations(spec, s):
    already_seen = set()
    for f in spec.parsed_fields():
        if f.is_array and not f.is_builtin:
            if f.base_type in already_seen:
                continue
            already_seen.add(f.base_type)
            # Field type may be fully qualified ("foo/Bar") or just
            # ("Bar") for a message defined in the same package as
            # `spec`. In the latter case, use spec.package.
            m = re.match(r'(\w+)/(\w+)', f.base_type)
            if m:
                pkg, msg_name = m.group(1), m.group(2)
                qualified = "{0}::msg::{1}".format(pkg, msg_name)
            else:
                pkg = spec.package
                qualified = "{0}::msg::{1}".format(pkg, f.base_type)
            # ROS 2 nests message types under a "::msg" sub-namespace,
            # so forward-declarations need to do the same.
            s.write("namespace {0} {{ namespace msg".format(pkg))
            s.write("\n{")
            s.write("inline bool operator== (const {0}& /*m1*/, const {0}& /*m2*/)".
                    format(qualified))
            s.write("{\n  return false;\n}\n\n} } // namespace ::msg")


def _to_snake_case(name):
    """CamelCase -> snake_case, used to derive ROS 2 header paths
    ("MyMsg" -> "my_msg")."""
    s1 = re.sub(r'(.)([A-Z][a-z]+)', r'\1_\2', name)
    return re.sub(r'([a-z0-9])([A-Z])', r'\1_\2', s1).lower()


def generate_file(pkg_path, pkg, msg, s=None):
    "Generate the definition file for a single message"

    if s is None:
        s = IndentedWriter()
    spec = get_msg_spec(pkg_path, pkg, msg)

    # ROS 2 headers use snake_case: pkg/msg/my_msg.hpp for MyMsg
    s.write('#include "{0}/msg/{1}.hpp"'.format(pkg, _to_snake_case(msg)))
    s.write("#include <boost/python.hpp>")
    s.write("#include <boost/python/suite/indexing/vector_indexing_suite.hpp>")
    s.write("#include \"nrp_general_library/datapack_interface/datapack.h\"\n")
    s.write("\n")

    generate_equality_forward_declarations(spec, s)

    # ROS 2 nests messages under <pkg>::msg; so do our helper functions
    s.write("namespace {0} {{ namespace msg".format(pkg))
    s.write("{")
    s.write("\n")

    s.write("using namespace boost::python;")
    s.write("using std::shared_ptr;")
    s.write("using std::vector;")
    s.write("\n")

    s.write("// Dummy equality check to avoid compilation error for vector_indexing_suite")
    s.write("bool operator== (const {0}& /*m1*/, const {0}& /*m2*/)".format(msg))
    s.write("{")
    with Indent(s, 2):
        s.write("return false;")
    s.write("}\n")

    generate_export_function(spec, s)

    s.write("} } // namespace ::msg")

    return s.getvalue()


def generate_package_file(pkg_path, pkg, s=None):
    "Generate the top level file for the given package with forward declarations for individual messages"
    if s is None:
        s = IndentedWriter()

    # Header
    s.write("#include <boost/python.hpp>\n")
    s.write("namespace {0} {{ namespace msg".format(pkg))
    s.write("{\n")

    # Forward declarations
    messages = list_msg_types(pkg_path, False)
    for m in messages:
        s.write("void export{0}();".format(m))

    # Boost python wrappers
    s.write("\n")
    s.write("BOOST_PYTHON_MODULE({0})".format(pkg))
    s.write("{")
    with Indent(s, 2):
        for m in messages:
            s.write("export{0}();".format(m))
    s.write("}\n")

    s.write("} } // namespace ::msg")
    return s.getvalue()


def generate_rospy_conversion(pkg_path, pkg, msg, s=None):
    if s is None:
        s = IndentedWriter()

    spec = get_msg_spec(pkg_path, pkg, msg)
    s.write("def {0}_to_ros(x):".format(msg))
    with Indent(s, 4):
        s.write("try:")
        with Indent(s, 4):
            s.write("m = {0}.msg.{1}()".format(pkg, msg))
            for f in spec.parsed_fields():
                def field_conversion(e):
                    f_conv = '{0}_to_ros'.format(f.base_type[f.base_type.find("/")+1:])
                    return e if f.is_builtin else '{0}({1})'.format(f_conv, e)

                if f.base_type in _ROS2_TIME_TYPES and f.is_array:
                    s.write("Conversion of time or duration array fields is not supported. Field {0} won't be converted"
                            .format(f.name))
                elif f.base_type in _ROS2_TIME_TYPES or f.type in ['time', 'duration']:
                    # ROS 2 builtin_interfaces/Time and /Duration use
                    # sec / nanosec (not ROS 1 secs / nsecs).
                    s.write("m.{0}.sec = x.{0}.sec".format(f.name))
                    s.write("m.{0}.nanosec = x.{0}.nanosec".format(f.name))
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

                if f.base_type in _ROS2_TIME_TYPES and f.is_array:
                    s.write("Conversion of time or duration array fields is not supported. Field {0} won't be converted"
                            .format(f.name))
                elif f.base_type in _ROS2_TIME_TYPES or f.type in ['time', 'duration']:
                    s.write("m.{0}.sec = x.{0}.sec".format(f.name))
                    s.write("m.{0}.nanosec = x.{0}.nanosec".format(f.name))
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
                error_msg = "\"'{0}_to_boost' must be called with objects of type '{1}.msg.{0}', " \
                            "called with object of type '{{}}'\"".format(msg, pkg)
                s.write("raise TypeError({}.format(type(x)))".format(error_msg))
            s.write("else:")
            with Indent(s, 4):
                s.write("raise e\n")

    return s.getvalue()


def generate_python_imports(pkg_path, pkg):
    pkgs = set()
    for m in list_msg_types(pkg_path, False):
        spec = get_msg_spec(pkg_path, pkg, m)
        for f in spec.parsed_fields():
            if f.is_header:
                pkgs.add('std_msgs')
            else:
                match = re.match(r'(\w+)/', f.base_type)
                if match and match.group(1) != pkg:
                    pkgs.add(match.group(1))
    return '\n'.join("from nrp_core.data.nrp_ros.{0}_conversions import *".format(p) for p in pkgs)


def write_rospy_conversions(pkg_path, pkg, target_dir, current_pkg):
    """
    Generate all rospy conversions.
    @param pkg_path: filesystem path to the ROS 2 package source tree
    @param pkg: We'll generate conversion functions for all messages defined in this ros package.
    @param current_pkg: The generated functions are being put into this ros package.
    """
    outfile = os.path.join(target_dir, pkg+'_conversions.py')
    with open(outfile, 'w') as f:
        f.write("import nrp_core.data.nrp_ros.rostime_boost_python\n")
        f.write(generate_python_imports(pkg_path, pkg))
        f.write("\nimport {0}.msg\n".format(pkg))
        f.write("import nrp_core.data.nrp_ros.{0} as cpp\n".format(pkg))

        for m in list_msg_types(pkg_path, False):
            f.write(generate_rospy_conversion(pkg_path, pkg, m))


def write_bindings(pkg_path, pkg, target_dir):
    "Generate and write all bindings"
    top_level_file = os.path.join(target_dir, pkg+'.cpp')
    with open(top_level_file, 'w') as f:
        f.write(generate_package_file(pkg_path, pkg))
    for m in list_msg_types(pkg_path, False):
        outfile = os.path.join(target_dir, "{0}_{1}.cpp".format(pkg, m))
        with open(outfile, 'w') as f:
            f.write(generate_file(pkg_path, pkg, m))


############################################################
# Helpers
############################################################

# ROS 1 had `time` and `duration` as primitive types. In ROS 2 they are
# full message types (builtin_interfaces/Time and /Duration), so they
# never reach MSG_TYPE_TO_CPP — a '/' in the typename means qualify()
# takes the pkg/Msg branch and emits pkg::msg::Msg.
MSG_TYPE_TO_CPP = {'byte': 'int8_t', 'char': 'uint8_t',
                   'bool': 'uint8_t',
                   'uint8': 'uint8_t', 'int8': 'int8_t',
                   'uint16': 'uint16_t', 'int16': 'int16_t',
                   'uint32': 'uint32_t', 'int32': 'int32_t',
                   'uint64': 'uint64_t', 'int64': 'int64_t',
                   'float32': 'float',
                   'float64': 'double',
                   'string': 'std::string'}


def qualify(name, current_pkg=None):
    """Return the C++ type name for a .msg field base type.

    ROS 2: "pkg/Msg" -> "pkg::msg::Msg"  (ROS 1 was pkg::Msg).
    Primitives are looked up in MSG_TYPE_TO_CPP. Non-primitive same-
    package references (no '/') are qualified with current_pkg when
    provided; otherwise KeyError is raised.
    """
    if '/' in name:
        pkg, msg = name.split('/', 1)
        return "{0}::msg::{1}".format(pkg, msg)
    if name in MSG_TYPE_TO_CPP:
        return MSG_TYPE_TO_CPP[name]
    if current_pkg is not None:
        return "{0}::msg::{1}".format(current_pkg, name)
    raise KeyError(name)


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
