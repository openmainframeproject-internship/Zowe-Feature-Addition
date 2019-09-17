This program and the accompanying materials are
made available under the terms of the Eclipse Public License v2.0 which accompanies
this distribution, and is available at https://www.eclipse.org/legal/epl-v20.html

SPDX-License-Identifier: EPL-2.0

Copyright Contributors to the Zowe Project.

# ZOWE Feature Additon Project 

## Overview

This repository contains the code for ZOWE feature additon project.



### IPL Explorer:
This application consists of two parts the ZSS IPL Plugin written in C and an angular app that is written in TypeScript. Code for the ZSS IPL plugin is inside *IPL-Explorer/zss-ipl-plugin* and code for Angular App is inside *IPL-Explorer/ipl-explorer-app*.

#### Building and deploying the ZSS-IPL-Plugin:
  * Copy the *zss-ipl-plugin* directory at the same level as *zlux-app-server*
  * Go to the build directory in zssServer/build
  * Run build.sh
  * Run deploy.sh
  * Restart your ZSS
  
##### Testing  ZSS-IPL-Plugin:
It is a plug-in that returns z/OS IPL information. Currently, it returns LOADxx, IEASYSxx, PROGxx and BPXPRMxx. After deploying use the following uri to get the required data
```
GET http://zsshost:zssport/ZLUX/plugins/org.zowe.ipl/services/explorer/_current/info
```
##### Visualization in IPL Explorer App:
The information can be visualize in an angular application. IPL-Explorer-App is one such application that is based on this plugin. 

#### Deploying the IPL-Explorer-App:
  * Copy the *ipl-explorer-app* directory at the same level as *zlux-app-server*
  * Copy the *org.zowe.ipl.explorer.json* file to *zlux-app-server/deploy/instance/ZLUX/plugins/*
  * Restart app server
  * Reload the page

### Zlux Editor

The work done during the internship focused on adding syntax highlighting support for BPXPRM and IEASYS datasets. The code is merged with the upstream zlux-editor [repo](https://github.com/zowe/zlux-editor).
The corresponding PR can be found [here](https://github.com/zowe/zlux-editor/pull/89).



This program and the accompanying materials are
made available under the terms of the Eclipse Public License v2.0 which accompanies
this distribution, and is available at https://www.eclipse.org/legal/epl-v20.html

SPDX-License-Identifier: EPL-2.0

Copyright Contributors to the Zowe Project.

