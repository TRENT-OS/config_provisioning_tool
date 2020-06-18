# Config Provisioning Tool
A tool to create a partition image that contains valid configuration server
data.This tool generates an NVM image from the XML system configuration. The XML system 
configuration contains XML configuration of the readable/writable parameters used in 
the system.

## Build

The build script needs to be called with 1 argument 

```bash
.
|____seos_sandbox
|     |
|     |____tools
|          |
|          |____cpt
|    build.sh
|    test.sh
```
## Build using: 
Config provision tool(cpt) gets built as part of build-sdk.sh script. If src is the folder
where the SEOS project is cloned, then cpt can be built as
 ```
 ./src/build.sh sdk 
 OR
 ./src/test.sh build
 ```
The resulting binary 'cpt' can be found in the below path:
OS-SDK/pkg/bin/cpt
and the build files


## Tool Usage
```
./config_provisioning_tool  <xml_file>
```
## Output file
It generates an NVM file(nvm_xx) where the cpt binary in called from.
