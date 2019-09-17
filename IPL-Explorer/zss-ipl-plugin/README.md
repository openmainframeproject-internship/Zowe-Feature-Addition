This program and the accompanying materials are
made available under the terms of the Eclipse Public License v2.0 which accompanies
this distribution, and is available at https://www.eclipse.org/legal/epl-v20.html

SPDX-License-Identifier: EPL-2.0

Copyright Contributors to the Zowe Project.

# ZSS IPL plug-in for Zowe

## Overview

A sample plug-in that allows:
* Returning z/OS IPL information. Currently, it returns LOADxx, IEASYSxx, PROGxx and BPXPRMxx.

```
GET http://zsshost:zssport/ZLUX/plugins/org.zowe.ipl/services/explorer/_current/info
```



### Getting the code:
* Use the following command to clone this repository with all the dependencies:
```
git clone --recursive git@github.com:UHaider/zss-ipl-plugin.git
```

### Building and deploying the ZSS plug-in:
  * Go to the build directory in zssServer/build
  * Run build.sh
  * Run deploy.sh
  * Restart your ZSS

### Visualization in IPL Explorer App:
There is another Angular application for zowe which diplays the returned data in a nice manner. Please see the [IPL Explorer Application](https://github.com/UHaider/ipl-explorer-app "IPL Exlorer App"). 

This program and the accompanying materials are
made available under the terms of the Eclipse Public License v2.0 which accompanies
this distribution, and is available at https://www.eclipse.org/legal/epl-v20.html

SPDX-License-Identifier: EPL-2.0

Copyright Contributors to the Zowe Project.
