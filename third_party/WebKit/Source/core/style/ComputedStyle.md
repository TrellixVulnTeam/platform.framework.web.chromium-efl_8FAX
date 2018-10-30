# Files:
Currently the C++ code for ComputedStyle lies in the following sets of files:
* ComputedStyleBase.{h,cpp} - This is the generated base class for ComputedStyle. All groups and fields are initialized here.
* ComputedStyle.{h,cpp} - These files add the following things on top of ComputedStyleBase:
	* Handwritten public accessors that are not straightforward and hence not generated in ComputedStyleBase.
	* Helper functions - e.g. functions that compare two ComputedStyle to check for equality across a set of fields.

The files that generate the code in ComputedStyleBase.{h,cpp} include:
* JSON files
* Python generator files
* Template files

## JSON files:
These files are inputs to the generator and tell the generator what shape our fields and functions take. This is a list of all the relevant JSON files (more detailed documentation can be found in the files themselves):
* [CSSProperties.json5](https://cs.chromium.org/chromium/src/third_party/WebKit/Source/core/css/CSSProperties.json5): Contains information for all the CSS properties we support.
* [ComputedStyleExtraFields.json5](https://cs.chromium.org/chromium/src/third_party/WebKit/Source/core/css/ComputedStyleExtraFields.json5): Specifies fields in ComputedStyle that we would like to generate, but are not CSS properties.
* [ComputedStyleDiffFunctions.json5](https://cs.chromium.org/chromium/src/third_party/WebKit/Source/core/css/ComputedStyleDiffFunctions.json5): Specifies the fields we want to diff in the various diff functions in ComputedStyle. It is used to generate the various diffing functions on ComputedStyle.

## Generator files:
These scripts generate the ComputedStyleBase.{h,cpp} files. This is a list of the relevant generator files (more detailed documentation can be found in the files themselves):
* [make_computed_style_base.py](https://cs.chromium.org/chromium/src/third_party/WebKit/Source/build/scripts/make_computed_style_base.py): Main file that handles the code generation logic. It consumes the JSON inputs provided and outputs fields and groups that encapsulate the structure of ComputedStyle.

## Template files:
For each generate file, the Python script uses a Jinja template file to format the output:
* [ComputedStyleBase.h.tmpl](https://cs.chromium.org/chromium/src/third_party/WebKit/Source/build/scripts/templates/ComputedStyleBase.h.tmpl): Header file with the field and group declarations.
* [ComputedStyleBase.cpp.tmpl](https://cs.chromium.org/chromium/src/third_party/WebKit/Source/build/scripts/templates/ComputedStyleBase.cpp.tmpl): Implementation of various generated helper functions. Notably, contains implementation of functions that use ComputedStyle such as the diffing functions.
* [ComputedStyleBaseConstants.h.tmpl](https://cs.chromium.org/chromium/src/third_party/WebKit/Source/build/scripts/templates/ComputedStyleBaseConstants.h.tmpl): Definitions of generated enums.
* [fields/*.tmpl](https://cs.chromium.org/chromium/src/third_party/WebKit/Source/build/scripts/templates/fields/): These files contain various macros that help us generate accessors for the fields depending on their field_template.

# Understanding where to make changes:
When we want to generate something in ComputedStyle, it’s usually as simple as adding a new entry to the JSON file or modifying a particular value. However, there can be cases where the thing to generate does not fit the current framework and more drastic changes are required. These changes can be done in several different places, but some are more preferable than others (most preferable to least preferable):
* C++: Can the code to be generated by refactored so that they can be generated easily? Example: renaming some classes to follow a naming pattern.
* Jinja templates: Allows changes to how the data from the generators are presented. Example: Adding an extra enum value at the end of generated enums to indicate their size.
* Python: Changes to the generator should try to be generic. Example: supporting mutable fields.
* JSON: The JSON file is changed when we need to provide more input information to the generator. This is least desirable because it exposes more complexity to contributors who want to perform simple tasks on the JSON.