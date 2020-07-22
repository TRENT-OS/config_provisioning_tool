# Config Provisioning Tool
This tool will create a provisioned image that contains valid
configuration data that can be used by the configuration service library.
The data is first parsed from an XML file and in turn written to the image file.

## Build
The Config Provisioning Tool (cpt) can be built by calling the provided
``build.sh`` script from the root folder and passing it the path to the SDK.
 ```
 src/build.sh <path-to-OS-SDK>
 ```

## Tool Usage
Run the tool by passing it the XML file to be parsed and specify the name of the
image file to be created. The filesystem that should be applied on this image
needs to be set to either "FAT" or "SPIFFS".
```
./cpt -i [<path-to-xml_file>] -o [<output_nvm_file_name>] -t [filesystem_type]
```
