# Config Provisioning Tool

This tool will create binary files that contain valid
configuration data that can be used by the configuration service library.
The data is first parsed from an XML file and in turn written to the files.

## Build

The Config Provisioning Tool (cpt) can be built by calling the provided
``build.sh`` script from the root folder and passing it the path to the SDK.

 ```shell
 src/build.sh <path-to-OS-SDK>
 ```

## Tool Usage

Run the tool by passing it the XML file to be parsed.

```shell
./cpt -i [<path-to-xml_file>]
```

The tool can also generate a partition image containing the binary files.
To create an image file, you need to additionally specify the desired output
image name and the filesystem type, that the image should be formatted to. The
possible options for the type are either "FAT" or "SPIFFS".
If the created partition image was formatted with a FAT filesystem, it can be
mounted like any other disk image under Linux.

```shell
./cpt -i [<path-to-xml_file>] -o [<output_nvm_file_name>] -t [<filesystem_type>]
```
