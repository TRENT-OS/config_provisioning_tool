# Config Provisioning Tool
This tool will create a provisioned partition image that contains valid
configuration data that can be used by the configuration service library.
The data is first parsed from an XML file and in turn written to a partition image.

## Build
The Config Provisioning Tool (cpt) can be built by calling the provided
``build.sh`` script from the root folder and passing it the path to the SDK.
 ```
 src/build.sh <path-to-OS-SDK>
 ```

## Tool Usage
Run the tool by passing it the XML file to be parsed and specify the name of the
image file to be created.
```
./cpt -i [<path-to-xml_file>] -o [<output_nvm_file_name>]
```
