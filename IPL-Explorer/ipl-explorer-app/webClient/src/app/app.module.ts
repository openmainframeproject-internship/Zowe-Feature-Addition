

/*
  This program and the accompanying materials are
  made available under the terms of the Eclipse Public License v2.0 which accompanies
  this distribution, and is available at https://www.eclipse.org/legal/epl-v20.html
  
  SPDX-License-Identifier: EPL-2.0
  
  Copyright Contributors to the Zowe Project.
*/

import { CommonModule } from '@angular/common';
import { NgModule, Inject } from '@angular/core';
import { FormsModule } from '@angular/forms';

import { ZluxButtonModule, ZluxPopupManagerModule } from '@zlux/widgets';
import { ReactiveFormsModule } from '@angular/forms';

import { AppComponent } from './app.component';
// import {MatTabsModule} from '@angular/material';
// import {MatExpansionModule} from '@angular/material/expansion';
//import {MatProgressSpinnerModule} from '@angular/material/progress-spinner';
// import {MatDividerModule} from '@angular/material/divider';
//  import {MatListModule} from '@angular/material/list';
// import { BrowserAnimationsModule } from '@angular/platform-browser/animations';

import { HttpClientModule } from '@angular/common/http';
// import { L10nConfig, L10nLoader, TranslationModule, StorageStrategy, ProviderType } from 'angular-l10n';
import { TranslationModule, L10nConfig, ISOCode, L10nLoader, LOCALE_CONFIG,
  TRANSLATION_CONFIG, LocaleConfig, TranslationConfig } from 'angular-l10n';
import { Angular2L10nConfig, Angular2InjectionTokens } from 'pluginlib/inject-resources';


const l10nConfig: L10nConfig = {
  translation: {
      providers: [],
      composedLanguage: [ISOCode.Language, ISOCode.Country],
      caching: true,
      missingValue: 'No key'
  }
};


@NgModule({
  declarations: [
    AppComponent
  ],
  imports: [
    // BrowserModule, /* remove this for within-MVD development */
    CommonModule,
    FormsModule,
    ZluxButtonModule,
    ZluxPopupManagerModule,
     HttpClientModule,
     ReactiveFormsModule,
    // MatTabsModule,
    // MatExpansionModule,
    // BrowserAnimationsModule,
    // MatDividerModule,
    // MatListModule,
    TranslationModule.forRoot(l10nConfig)
  ],
  // exports: [

  //   MatTabsModule,
  //   MatExpansionModule,
  //   BrowserAnimationsModule,
  //   MatDividerModule,
  //   MatListModule,
  // ],
  providers: [],
  bootstrap: [AppComponent]
})
export class AppModule { 
  constructor(
    private l10nLoader: L10nLoader,
    @Inject(Angular2InjectionTokens.L10N_CONFIG) private l10nConfig: Angular2L10nConfig,
    @Inject(LOCALE_CONFIG) private localeConfig: LocaleConfig,
    @Inject(TRANSLATION_CONFIG) private translationConfig: TranslationConfig,

  ) {
    this.localeConfig.defaultLocale = this.l10nConfig.defaultLocale;
    this.translationConfig.providers = this.l10nConfig.providers;
    this.l10nLoader.load();
  }
}


/*
  This program and the accompanying materials are
  made available under the terms of the Eclipse Public License v2.0 which accompanies
  this distribution, and is available at https://www.eclipse.org/legal/epl-v20.html
  
  SPDX-License-Identifier: EPL-2.0
  
  Copyright Contributors to the Zowe Project.
*/

