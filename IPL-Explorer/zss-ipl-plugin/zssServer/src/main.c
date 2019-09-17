

/*
  This program and the accompanying materials are
  made available under the terms of the Eclipse Public License v2.0 which accompanies
  this distribution, and is available at https://www.eclipse.org/legal/epl-v20.html

  SPDX-License-Identifier: EPL-2.0

  Copyright Contributors to the Zowe Project.
*/

#include <stddef.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

#include <limits.h>
#include <errno.h>

#include "zowetypes.h"
#include "alloc.h"
#include "utils.h"
#include "collections.h"
#include "bpxnet.h"
#include "socketmgmt.h"
#include "le.h"
#include "json.h"
#include "httpserver.h"
#include "dataservice.h"
#include "serviceUtils.h"
#include "utils.h"
#include "zos.h"

#include "zis/client.h"
#include "zis/service.h"

#include "iplServe.h"


static void printIPL(jsonPrinter *printer) {
  int temp;
  temp = printIPLData(printer);
}

static int serveIPLInfo(HttpService *service, HttpResponse *response) {

  HttpRequest *request = response->request;
  if (strcmp(request->method, methodGET)) {
    respondWithError(response, HTTP_STATUS_BAD_REQUEST, "Unsupported request");
    return 0;
  }

  jsonPrinter *printer = respondWithJsonPrinter(response);
  setResponseStatus(response, HTTP_STATUS_OK, "Ok");
  setDefaultJSONRESTHeaders(response);
  writeHeader(response);

  jsonStart(printer);
  printIPL(printer);
  jsonEnd(printer);

  finishResponse(response);

  return 0;
}

static char *getURLSegment(HttpRequest *request, int number) {

  StringListElt *currSegment = firstStringListElt(request->parsedFile);

  char *segment = NULL;
  for (int i = 0; i < number; i++) {
    if (currSegment == NULL) {
      return NULL;
    }
    segment = currSegment->string;
    currSegment = currSegment->next;
  }

  return segment;
}

static char *getFirstServicePathSegment(HttpRequest *request) {
  // skip ZLUX/plugins/org.zowe.ipl/services/explorer/{service}
  return getURLSegment(request, 7);
}

static short getASIDParm(HttpRequest *request) {

  char *startASIDString = getFirstServicePathSegment(request);
  if (startASIDString == NULL) {
    return -1;
  }

  char *endASIDString = NULL;
  errno = 0;
  long asid = strtol(startASIDString, &endASIDString, 16);

  if (startASIDString == endASIDString || errno != 0) {
    return -1;
  }

  if (asid < SHRT_MIN || SHRT_MAX < asid) {
    return -1;
  }

  return asid;
}


static char *getServiceEndpoint(HttpRequest *request) {
  // skip ZLUX/plugins/org.zowe.ipl/services/explorer
  return getURLSegment(request, 6);
}

static int serveIPLData(HttpService *service, HttpResponse *response) {

  HttpRequest *request = response->request;

  char *serviceName = getServiceEndpoint(request);
  if (!strcmp(serviceName, "info")) {
    return serveIPLInfo(service, response);
  }
  
  respondWithError(response, HTTP_STATUS_NOT_FOUND, "Service not found");

  return 0;
}

#pragma export(iplExplorerPluginServiceInstaller)

int iplExplorerPluginServiceInstaller(DataService *dataService,
                                    HttpServer *server) {

  // ZLUX/plugins/org.zowe.ipl/services/explorer/**
  HttpService *service = makeHttpDataService(dataService, server);
  service->serviceFunction = serveIPLData;
  service->runInSubtask = FALSE;

  return 0;
}


/*
  This program and the accompanying materials are
  made available under the terms of the Eclipse Public License v2.0 which accompanies
  this distribution, and is available at https://www.eclipse.org/legal/epl-v20.html

  SPDX-License-Identifier: EPL-2.0

  Copyright Contributors to the Zowe Project.
*/

