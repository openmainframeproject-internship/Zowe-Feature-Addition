

/*
  This program and the accompanying materials are
  made available under the terms of the Eclipse Public License v2.0 which accompanies
  this distribution, and is available at https://www.eclipse.org/legal/epl-v20.html
  
  SPDX-License-Identifier: EPL-2.0
  
  Copyright Contributors to the Zowe Project.
*/

import { Component, Inject } from '@angular/core';
import { Angular2InjectionTokens } from 'pluginlib/inject-resources';

import { ZluxPopupManagerService, ZluxErrorSeverity } from '@zlux/widgets';
import { FormBuilder, Validators } from "@angular/forms";

import { Http, Response } from '@angular/http'

import { LocaleService, TranslationService, Language } from 'angular-l10n';

@Component({
  selector: 'app-root',
  templateUrl: './app.component.html',
  styleUrls: ['./app.component.css'],
  providers: [ZluxPopupManagerService]
})

export class AppComponent {
  @Language() lang: string;

  targetAppId: string = "org.zowe.editor";
  callStatus: string;
  LOADxx: any;
  BPXPRMxx: any;
  IEASYSxx: any;
  PROGxx: any;
  mainSelection: boolean = false;
  dataLoading: boolean = true;
  Options: any = ['LOADxx', 'BPXPRMxx', 'IEASYSxx', 'PROGxx']

  parameters: string =
    `{"type": "openFile", "name": "path"}`;

  
  actionType: string = "Launch";
  targetMode: string = "PluginCreate";
  items = ['a', 'b', 'c', 'd']
  helloText: any;
  serverResponseMessage: string;
  myUrl: any;
  test: any;
  select: any;
  arrayMember: any = ['LOADxx', 'BPXPRMxx', 'IEASYSxx', 'PROGxx']
  SubOptions: any = []
  memberSelected: any = [];
  dataarray: any;
  jsonData: any;
  value: number;
  currindex: any;
  dup_array: any = [];

  constructor(
    public locale: LocaleService,
    public fb: FormBuilder,
    public translation: TranslationService,
    @Inject(Angular2InjectionTokens.PLUGIN_DEFINITION) private pluginDefinition: ZLUX.ContainerPluginDefinition,
    @Inject(Angular2InjectionTokens.LOGGER) private log: ZLUX.ComponentLogger,
    @Inject(Angular2InjectionTokens.LAUNCH_METADATA) private launchMetadata: any,
    private popupManager: ZluxPopupManagerService,
    private http: Http
  ) {
    //is there a better way so that I can get this info into the HelloService constructor instead of calling a set method directly after creation???
    this.popupManager.setLogger(log);
    this.value = 0;
    if (this.launchMetadata != null && this.launchMetadata.data != null && this.launchMetadata.data.type != null) {
      this.handleLaunchOrMessageObject(this.launchMetadata.data);
    }
    this.test = this.pluginDefinition.getBasePlugin();
    // this.myUrl= ZoweZLUX.uriBroker.pluginRESTUri(this.pluginDefinition.getBasePlugin(), 'explorer', 'info');
    this.myUrl = "/ZLUX/plugins/org.zowe.ipl/services/explorer/_current/info"
    setTimeout(() => {
      this.log.info(`Sending GET request to ${this.myUrl}`);
      this.http.get(this.myUrl).map(res => res.json()).subscribe(
        data => {
          console.log(data)
          this.dataLoading = false;
          this.handledata(data)


        },
        error => {
          console.log(error)
          this.log.warn(`Error from GET. error=${error}`);
          this.helloText = error;
        }
      );
    }, 2000);

  }


  callExternalEvent() {
    let ccDataset = this.memberSelected.Dataset.split(':')[0]
    let ccVolume = this.memberSelected.Dataset.split(':')[1]
    let ccMember = this.memberSelected.Member;
    var parameters = null;
    const popupOptions = {
      blocking: true,
      buttons: [this.translation.translate('close')]
    };
    /*Parameters for Actions could be a number, string, or object. The actual event context of an Action that an App recieves will be an object with attributes filled in via these parameters*/
    try {
      if (this.parameters !== undefined && this.parameters.trim() !== "") {
        parameters = JSON.parse(this.parameters);
      }
    } catch (e) {
      //this.parameters was not JSON
    }
    if (this.targetAppId) {
      let message = '';
      /* 
         With ZLUX, there's a global called ZoweZLUX which holds useful tools. So, a site
         Can determine what actions to take by knowing if it is or isnt embedded in ZLUX via IFrame.
      */
      /* PluginManager can be used to find what Plugins (Apps are a type of Plugin) are part of the current ZLUX instance.
         Once you know that the App you want is present, you can execute Actions on it by using the Dispatcher.
      */
      let dispatcher = ZoweZLUX.dispatcher;
      let pluginManager = ZoweZLUX.pluginManager;
      let plugin = pluginManager.getPlugin(this.targetAppId);
      if (plugin) {
        let type = dispatcher.constants.ActionType[this.actionType];
        let mode = dispatcher.constants.ActionTargetMode[this.targetMode];

        if (type != undefined && mode != undefined) {
          let actionTitle = 'Launch app from sample app';
          let actionID = 'org.zowe.zlux.sample.launch';
          let argumentFormatter = { data: { op: 'deref', source: 'event', path: ['data'] } };
          /*Actions can be made ahead of time, stored and registered at startup, but for example purposes we are making one on-the-fly.
            Actions are also typically associated with Recognizers, which execute an Action when a certain pattern is seen in the running App.
          */
          let action = dispatcher.makeAction(actionID, actionTitle, mode, type, this.targetAppId, argumentFormatter);
          let argumentData = { 'data': (parameters ? parameters : this.parameters) };
          this.log.info((message = this.translation.translate('request_succeeded'))); // App request succeeded
          this.callStatus = message;
          /*Just because the Action is invoked does not mean the target App will accept it. We've made an Action on the fly,
            So the data could be in any shape under the "data" attribute and it is up to the target App to take action or ignore this request*/
          dispatcher.invokeAction(action, argumentData);
        } else {
          this.log.warn((message = 'Invalid target mode or action type specified'));
        }
      } else {
        this.popupManager.reportError(
          ZluxErrorSeverity.WARNING,
          this.translation.translate('invalid_plugin_identifier'), // 
          `${this.translation.translate('no_plugin_found_for_identifier')} ${this.targetAppId}`, popupOptions);
      }

      this.callStatus = message;
    }
  }


  changeMember(e) {
    console.log(e)
    this.select = e;
    this.mainSelection = true;
    this.dataarray = this.jsonData[this.select];
    this.SubOptions = this.jsonData[this.select];
    this.memberSelected = this.SubOptions[0];

  }
  changesubOptions(e) {
 
    for (var i = 0, len = this.SubOptions.length; i < len; ++i) {
      this.dup_array[i] = this.SubOptions[i].Member;
    }
    console.log(this.dup_array)
    this.currindex = this.dup_array.indexOf(e);
    console.log(this.currindex)
    this.memberSelected = this.SubOptions[this.currindex]

  }

  selectedOption(i, optionselected) {
    this.memberSelected = this.SubOptions[i]
  }

  handledata(data) {
    this.jsonData = data;
    this.LOADxx = data['LOADxx'];
    console.log(this.LOADxx);

    this.BPXPRMxx = data.BPXPRMxx;
    console.log(this.BPXPRMxx);

    this.IEASYSxx = data.IEASYSxx;
    this.PROGxx = data.PROGxx;
  }


  handleLaunchOrMessageObject(data: any) {
    switch (data.type) {
      case 'setAppRequest':
        let actionType = data.actionType;
        let msg: string;
        if (actionType == 'Launch' || actionType == 'Message') {
          let mode = data.targetMode;
          if (mode == 'PluginCreate' || mode == 'PluginFindAnyOrCreate') {
            this.actionType = actionType;
            this.targetMode = mode;
            this.targetAppId = data.targetAppId;
            this.parameters = data.requestText;
          } else {
            msg = `Invalid target mode given (${mode})`;
            this.log.warn(msg);
            this.callStatus = msg;
          }
        } else {
          msg = `Invalid action type given (${actionType})`;
          this.log.warn(msg);
          this.callStatus = msg;
        }
        break;
      default:
        this.log.warn(`Unknown command (${data.type}) given in launch metadata.`);
    }
  }

  /* I expect a JSON here*/
  zluxOnMessage(eventContext: any): Promise<any> {
    return new Promise((resolve, reject) => {
      if (eventContext != null && eventContext.data != null && eventContext.data.type != null) {
        resolve(this.handleLaunchOrMessageObject(eventContext.data));
      } else {
        let msg = 'Event context missing or malformed';
        this.log.warn('onMessage ' + msg);
        return reject(msg);
      }
    });
  }


  provideZLUXDispatcherCallbacks(): ZLUX.ApplicationCallbacks {
    return {
      onMessage: (eventContext: any): Promise<any> => {
        return this.zluxOnMessage(eventContext);
      }
    }
  }


}


/*
  This program and the accompanying materials are
  made available under the terms of the Eclipse Public License v2.0 which accompanies
  this distribution, and is available at https://www.eclipse.org/legal/epl-v20.html

  SPDX-License-Identifier: EPL-2.0

  Copyright Contributors to the Zowe Project.
*/

