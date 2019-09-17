#include "iplServe.h"
#include "json.h"
#define NUM_ALPHA 81


int * __ptr32 getCVTs(void) {
	int * __ptr32 mem = (int * __ptr32) 0;
	int * __ptr32 theCVT = (int * __ptr32)(*(mem + (0x10 / 4)));
	return theCVT;
}

int * __ptr32 getECVTs(void) {
	int * __ptr32 mem = getCVTs();
	int * __ptr32 theECVT = (int * __ptr32)(*(mem + (0x8C / 4)));
	return theECVT;
}

int * __ptr32 getECVTIPAs(void) {
	int * __ptr32 mem = getECVTs();
	int * __ptr32 theECVTIPA = (int * __ptr32)(*(mem + (0x188 / 4)));
	return theECVTIPA;
}

int displaymem(jsonPrinter *printer, char *member, char*pliba)
{
	FILE *stream;
	char buffer[NUM_ALPHA];
	buffer[NUM_ALPHA - 1] = '\0';

	int done = 0;
	char fnamepre[48];
	char *fnamepost = malloc(sizeof(char) * (strlen(member) + 4));
    if (fnamepost == NULL)
    {
        return 0;
    }
	char *dataset;
	dataset = (char *)malloc(59);
    if (dataset == NULL)
    {
        free(fnamepost);
        return 0;
    }
	char pname[45];
	char *saveptr1;
	char *plibarray = malloc(sizeof(char) * (strlen(pliba) + 1));
    if (plibarray == NULL)
    {
        free(fnamepost);
        free(dataset);
        return 0;
    }

	memset(fnamepost, '\0', sizeof(fnamepost));
	strcat(fnamepost, "(");
	strcat(fnamepost, member);
	strcat(fnamepost, ")'");

	strcpy(plibarray, pliba);
	char* tokenp = strtok_r(plibarray, "-", &saveptr1);

	while (tokenp != NULL) {
		memset(pname, '\0', sizeof(pname));
		strncpy(pname, tokenp, 44);
		//make a valid filename
		memset(fnamepre, '\0', sizeof(fnamepre));
		strcat(fnamepre, "//'");
		strcat(fnamepre, pname);

		memset(dataset, '\0', sizeof(dataset));
		strcat(dataset, fnamepre);
		strcat(dataset, fnamepost);

		if ((stream = fopen(dataset, "r")) != NULL)
		{
			done = 1;
            jsonStartObject(printer, NULL);
			jsonAddString(printer, "Dataset", tokenp);
			jsonAddString(printer, "Member", member);

            //Create a Json array for contents
			jsonStartArray(printer, "Lines");
			//jsonStartObject(printer, NULL);

			while ((fgets(buffer, NUM_ALPHA, stream)) != NULL)
			{
				//For Json object
				jsonAddString(printer, NULL, buffer);
			}
            //jsonEndObject(printer);
			jsonEndArray(printer);
			
			jsonEndObject(printer);
			fclose(dataset);
			break;
		}

		tokenp = strtok_r(NULL, "-", &saveptr1);
	}
    free(dataset);
	free(fnamepost);
	free(plibarray);
    return done;
}

int showomvs(jsonPrinter *printer, char *allomvs, char*pliba)
{
    int done=0;
    char *saveptr1;
	char *pre = "BPXPRM";
	char *name = malloc(sizeof(char) * (strlen(pre) + 3));
    if (name == NULL)
    {
        return 0;
    }
	char *progarr = malloc(sizeof(char) * (strlen(allomvs) + 1));
    if (progarr == NULL)
    {
        free(name);
        return 0;
    }
	memset(progarr, '\0', (strlen(allomvs) + 1));
	strcpy(progarr, allomvs);

	char* tokens = strtok_r(progarr, ", ", &saveptr1);
	while (tokens != NULL)
	{
		//remove leading whitespaces
		while (isspace((unsigned char)*tokens))
			tokens++;

		if (strlen(tokens) == 2)
		{
			memset(name, '\0', sizeof(name));
			strcat(name, pre);
			strcat(name, tokens);
			done=displaymem(printer, name, pliba);
		}

		tokens = strtok_r(NULL, ",", &saveptr1);
	}

	free(progarr);
	free(name);
    return done;
}

int readomvs(jsonPrinter *printer, char *finame, char *pliba)
{
	FILE *stream;
	char buffer[NUM_ALPHA];
	buffer[NUM_ALPHA - 1] = '\0';
	char *delim1 = "OMVS=(";
	char *delim2 = "OMVS=";
	char *result;
	char *subresult;
	char *bpxs = NULL;
	int omvsdef = 0;

	if ((stream = fopen(finame, "r")) != NULL)
	{
		while ((fgets(buffer, NUM_ALPHA, stream)) != NULL)
		{
			//Multiple value parameter
			result = strstr(buffer, delim1);
			if (result != NULL)
			{
				subresult = strstr(result, ")");
				if (subresult != NULL)
				{
					char *tmp = realloc(bpxs, sizeof(char) * (subresult - (result + 6) + 1));
					if (tmp)
					{
						bpxs = tmp;
						memset(bpxs, '\0', (subresult - (result + 6) + 1));
						strncpy(bpxs, result + 6, (subresult - (result + 6)));
					}
                    else
                    {
                        return 0;
                    }
                    
					omvsdef = 1;
					//break the outer loop
					break;
				}
				else
				{
					subresult = strstr(result, " ");
					if (subresult != NULL)
					{
						char *tmp = realloc(bpxs, sizeof(char) * (subresult - (result + 6) + 1));
						if (tmp)
						{
							bpxs = tmp;
							memset(bpxs, '\0', (subresult - (result + 6) + 1));
							strncpy(bpxs, result + 6, (subresult - (result + 6)));
						}
                        else
                        {
                            return 0;
                        }
                        

						while ((fgets(buffer, NUM_ALPHA, stream)) != NULL)
						{
							result = strstr(buffer, ")");
							if (result != NULL)
							{

								int sizeadd = (int)result - (int)buffer;
								int newsize = (sizeadd + strlen(bpxs) + 1);
								char *tmp = realloc(bpxs, sizeof(char) * newsize);
								if (tmp)
								{
									bpxs = tmp;
									memset(bpxs + strlen(bpxs), '\0', sizeadd + 1);
									strncpy(bpxs + strlen(bpxs), buffer, sizeadd);
								}
                                else
                                {
                                    return 0;
                                }
                                
								omvsdef = 1;
								//break inner loop
								break;
							}
							else
							{
								result = strstr(buffer, ", ");
								if (result != NULL)
								{
									int sizeadd = (int)result - (int)buffer + 1;
									int newsize = (sizeadd + strlen(bpxs) + 1);
									char *tmp = realloc(bpxs, sizeof(char) * newsize);
									if (tmp)
									{
										bpxs = tmp;
										memset(bpxs + strlen(bpxs), '\0', sizeadd + 1);
										strncpy(bpxs + strlen(bpxs), buffer, sizeadd);
									}
                                    else
                                    {
                                        return 0;
                                    }
                                    
								}
								else
								{
									//printf("Syntax is ill \n");
                                    return 0;
								}
							}
						}
						
						//printf("Multi omvs 2 %s \n", bpxs);
						//break outer loop
						break;

					}
					else
					{
						//printf("Syntax is ill\n");
                        return 0;
					}
				}

			}
			//Single value parameter
			else
			{
				result = strstr(buffer, delim2);
				if (result != NULL)
				{
					char *tmp = realloc(bpxs, sizeof(char) * 4);
					if (tmp)
					{
						bpxs = tmp;
						memset(bpxs, '\0', 4);
						strncpy(bpxs, result + 5, 3);
					}
                    else
                    {
                        return 0;
                    }
                    
					//printf("OMVS %s \n", bpxs);
					omvsdef = 1;
					//break the outer loop
					break;
				}

			}

		}
		fclose(stream);
	}
	if (omvsdef == 1)
	{
        int omvsReturned;
		omvsReturned=showomvs(printer, bpxs, pliba);
	}
	free(bpxs);
	return omvsdef;
}

int josnArrayBPXPRM(jsonPrinter *printer, char *pliba, char *syspa)
{
	char pname[45];
	int done = 0;
	char fnamepre[48];
	char fnamepost[11];
    char *saveptr1, *saveptr2;
	char *dataset;

    jsonStartArray(printer,"BPXPRMxx");

    dataset = (char *)malloc(59);
    if (dataset == NULL)
    {
        jsonEndArray(printer);
        return 0;
    }
	
	char *plibarray = malloc(sizeof(char) * (strlen(pliba) + 1));
    if (plibarray == NULL)
    {
        jsonEndArray(printer);
        free(dataset);
        return 0;
    }

	char *sysparm = malloc(sizeof(char) * (strlen(syspa) + 1));
	if (sysparm == NULL)
    {
        jsonEndArray(printer);
        free(dataset);
        free(plibarray);
        return 0;
    }

	strcpy(sysparm, syspa);
	char* tokens = strtok_r(sysparm, ",", &saveptr1);

	//Loop over IEASYSxx
	while (tokens != NULL) {

		if (strlen(tokens) > 1)
		{
			memset(fnamepost, '\0', sizeof(fnamepost));
			strcat(fnamepost, "(IEASYS");
			strcat(fnamepost, tokens);
			strcat(fnamepost, ")'");

			memset(pname, '\0', sizeof(pname));
			strcpy(plibarray, pliba);
			char* tokenp = strtok_r(plibarray, "-", &saveptr2);

			while (tokenp != NULL) {
				memset(pname, '\0', sizeof(pname));
				strncpy(pname, tokenp, 44);
				//make a valid filename
				memset(fnamepre, '\0', sizeof(fnamepre));
				strcat(fnamepre, "//'");
				strcat(fnamepre, pname);

				memset(dataset, '\0', sizeof(dataset));
				strcat(dataset, fnamepre);
				strcat(dataset, fnamepost);
				done = readomvs(printer, dataset, pliba);
				if (done == 1)
				{
					break;
				}
				tokenp = strtok_r(NULL, "-", &saveptr2);
			}

			if (done == 1)
			{
				break;
			}
		}
		tokens = strtok_r(NULL, ",", &saveptr1);
	}

    jsonEndArray(printer);
	free(dataset);
	free(sysparm);
	free(plibarray);
    return done;
}

int showprog(jsonPrinter *printer, char *allprogs, char*pliba)
{
    int done=0;
    char *saveptr1;
	char *pre = "PROG";
	char *name = malloc(sizeof(char) * (strlen(pre) + 3));
    if (name == NULL)
    {
        return 0;
    }
	char *progarr = malloc(sizeof(char) * (strlen(allprogs) + 1));
    if (progarr == NULL)
    {
        free(name);
        return 0;
    }
	memset(progarr, '\0', (strlen(allprogs) + 1));
	strcpy(progarr, allprogs);

	char* tokens = strtok_r(progarr, ", ", &saveptr1);
	while (tokens != NULL)
	{
		//remove leading whitespaces
		while (isspace((unsigned char)*tokens))
			tokens++;

		if (strlen(tokens) == 2)
		{
			memset(name, '\0', sizeof(name));
			strcat(name, pre);
			strcat(name, tokens);
			done=displaymem(printer, name, pliba);
		}

		tokens = strtok_r(NULL, ",", &saveptr1);
	}

	free(progarr);
	free(name);
    return done;
}

int readprog(jsonPrinter *printer, char *finame, char *pliba)
{
	FILE * stream;
	char buffer[NUM_ALPHA];
	buffer[NUM_ALPHA - 1] = '\0';
	char *delim1 = "PROG=(";
	char *delim2 = "PROG=";
	char *result;
	char *subresult;
	char *progs = NULL;
	int progsdef = 0;

	if ((stream = fopen(finame, "r")) != NULL)
	{
		while ((fgets(buffer, NUM_ALPHA, stream)) != NULL)
		{
			//Multiple value parameter
			result = strstr(buffer, delim1);
			if (result != NULL)
			{
				subresult = strstr(result, ")");
				if (subresult != NULL)
				{
					//printf("Length %d \n", subresult - (result+6));
					char *tmp = realloc(progs, sizeof(char) * (subresult - (result + 6) + 1));
					if (tmp)
					{
						progs = tmp;
						memset(progs, '\0', (subresult - (result + 6) + 1));
						strncpy(progs, result + 6, (subresult - (result + 6)));
					}
                    else
                    {
                        return 0;
                    }
                    
					//printf("PROGS same line %s \n", progs);
					progsdef = 1;
					//break the outer loop
					break;
				}
				else
				{
					subresult = strstr(result, " ");
					if (subresult != NULL)
					{
						char *tmp = realloc(progs, sizeof(char) * (subresult - (result + 6) + 1));
						if (tmp)
						{
							progs = tmp;
							memset(progs, '\0', (subresult - (result + 6) + 1));
							strncpy(progs, result + 6, (subresult - (result + 6)));
						}
                        else
                        {
                            return 0;
                        }
                        

						while ((fgets(buffer, NUM_ALPHA, stream)) != NULL)
						{
							result = strstr(buffer, ")");
							if (result != NULL)
							{

								int sizeadd = (int)result - (int)buffer;
								int newsize = (sizeadd + strlen(progs) + 1);
								char *tmp = realloc(progs, sizeof(char) * newsize);
								if (tmp)
								{
									progs = tmp;
									memset(progs + strlen(progs), '\0', sizeadd + 1);
									strncpy(progs + strlen(progs), buffer, sizeadd);
								}
                                else
                                {
                                    return 0;
                                }
                                
								progsdef = 1;
								//break inner loop
								break;
							}
							else
							{
								result = strstr(buffer, ", ");
								if (result != NULL)
								{
									int sizeadd = (int)result - (int)buffer + 1;
									int newsize = (sizeadd + strlen(progs) + 1);
									char *tmp = realloc(progs, sizeof(char) * newsize);
									if (tmp)
									{
										progs = tmp;
										memset(progs + strlen(progs), '\0', sizeadd + 1);
										strncpy(progs + strlen(progs), buffer, sizeadd);
									}
                                    else
                                    {
                                        return 0;
                                    }
                                    
								}
								else
								{
									//printf("Syntax is ill \n");
                                    return 0;
								}
							}
						}

						//break outer loop
						break;

					}
					else
					{
						//printf("Syntax is ill\n");
                        return 0;
					}
				}
			}
			//Single value parameter
			else
			{
				result = strstr(buffer, delim2);
				if (result != NULL)
				{
					char *tmp = realloc(progs, sizeof(char) * 4);
					if (tmp)
					{
						progs = tmp;
						memset(progs, '\0', 4);
						strncpy(progs, result + 5, 3);
					}
                    else
                    {
                        return 0;
                    }
                    
					progsdef = 1;
					//break the outer loop
					break;
				}

			}

		}
		fclose(stream);
	}
	if (progsdef == 1)
	{
		//For Json object
		//printf("Progsxx: %s \n", progs);
        int progReturned;
		progReturned = showprog(printer, progs, pliba);
	}
	free(progs);
	return progsdef;
}

int josnArrayPROGxx(jsonPrinter *printer, char *pliba, char *syspa)
{
	char pname[45];
	int done = 0;
	char fnamepre[48];
	char fnamepost[11];
	char *dataset;
    char *saveptr1, *saveptr2;

    jsonStartArray(printer, "PROGxx");

	dataset = (char *)malloc(59);
    if (dataset == NULL)
    {
        jsonEndArray(printer);
        return 0;
    }
	
	char *plibarray = malloc(sizeof(char) * (strlen(pliba) + 1));
    if (plibarray == NULL)
    {
        jsonEndArray(printer);
        free(dataset);
        return 0;
    }
	char *sysparm = malloc(sizeof(char) * (strlen(syspa) + 1));
     if (sysparm == NULL)
    {
        jsonEndArray(printer);
        free(dataset);
        free(plibarray);
        return 0;
    }

	strcpy(sysparm, syspa);
	char* tokens = strtok_r(sysparm, ",", &saveptr1);

	//Loop over IEASYSxx
	while (tokens != NULL) {

		if (strlen(tokens) > 1)
		{
			memset(fnamepost, '\0', sizeof(fnamepost));
			strcat(fnamepost, "(IEASYS");
			strcat(fnamepost, tokens);
			strcat(fnamepost, ")'");

			memset(pname, '\0', sizeof(pname));
			strcpy(plibarray, pliba);
			char* tokenp = strtok_r(plibarray, "-", &saveptr2);

			while (tokenp != NULL) {
				memset(pname, '\0', sizeof(pname));
				strncpy(pname, tokenp, 44);
				//make a valid filename
				memset(fnamepre, '\0', sizeof(fnamepre));
				strcat(fnamepre, "//'");
				strcat(fnamepre, pname);

				memset(dataset, '\0', sizeof(dataset));
				strcat(dataset, fnamepre);
				strcat(dataset, fnamepost);
				done = readprog(printer, dataset, pliba);
				if (done == 1)
				{
					break;
				}

				tokenp = strtok_r(NULL, "-", &saveptr2);
			}
			if (done == 1)
			{
				break;
			}
		}
		tokens = strtok_r(NULL, ",", &saveptr1);
	}
    
    jsonEndArray(printer);
	free(dataset);
	free(sysparm);
	free(plibarray);
    return done;
}

int josnArrayIEASYSxx(jsonPrinter *printer, char *pliba, char *syspa)
{
	FILE *stream;
	char buffer[NUM_ALPHA];
	buffer[NUM_ALPHA - 1] = '\0';
	char pname[45];
	int done = 0;
	char fnamepre[48];
    char *saveptr1;
    char *pre = "IEASYS";

    jsonStartArray(printer, "IEASYSxx");

	char *name = malloc(sizeof(char) * (strlen(pre) + 3));
    if (name == NULL)
    {
        jsonEndArray(printer);
        return 0;
    }
    char *dataset;
	dataset = (char *)malloc(59);
    if (dataset == NULL)
    {
        jsonEndArray(printer);
        free(name);
        return 0;
    }
	
	char *plibarray = malloc(sizeof(char) * (strlen(pliba) + 1));
    if (plibarray == NULL)
    {
        jsonEndArray(printer);
        free(dataset);
        return 0;
    }
	char *sysparm = malloc(sizeof(char) * (strlen(syspa) + 1));
    if (sysparm == NULL)
    {
        jsonEndArray(printer);
        free(dataset);
        free(plibarray);
        return 0;
    }
	strcpy(sysparm, syspa);

	char* tokens = strtok_r(sysparm, ",", &saveptr1);
	//Loop over IEASYSxx
	while (tokens != NULL) {

		if (strlen(tokens) > 1)
		{
            //remove leading whitespaces
            while (isspace((unsigned char)*tokens))
            {
                tokens++;
            }
            if (strlen(tokens) == 2)
            {
                memset(name, '\0', sizeof(name));
                strcat(name, pre);
                strcat(name, tokens);
                done = displaymem(printer, name, pliba);
            }
		}
		tokens = strtok_r(NULL, ",", &saveptr1);
	}
    jsonEndArray(printer);
    free(name);
	free(dataset);
	free(sysparm);
	free(plibarray);
    return done;
}

int getsysparm(char **sysparm, char *finame)
{
	FILE *stream;
	char buffer[NUM_ALPHA];
	buffer[NUM_ALPHA - 1] = '\0';
	char delim[] = "SYSPARM";
	char pname[45];
	int sysparmlen;

	int ROWS = 0;
	int COLS = 53;
	char tok;
	char *plibarr = NULL;
	char *token = malloc(sizeof(char) * strlen(delim));
    if (token == NULL)
    {
        *sysparm = NULL;
        return 0;
    }

	if ((stream = fopen(finame, "r")) != NULL)
	{
		while ((fgets(buffer, NUM_ALPHA, stream)) != NULL)
		{

			memset(token, '\0', sizeof(token));
			strncpy(token, buffer, strlen(delim));
			if (strcmp(token, delim) == 0)
			{
				memset(pname, '\0', sizeof(pname));
				ROWS = ROWS + 1;
				char *tmp = realloc(plibarr, sizeof(char) * (ROWS*COLS));
				if (tmp)
				{
					plibarr = tmp;
					memset(plibarr + ((ROWS - 1)*COLS), '\0', sizeof(char)* COLS);
					strncpy(pname, buffer + 9, 44);
					sysparmlen = strlen(pname);
					tok = pname[0];
					if (tok == '(') {
						memset(pname, '\0', sizeof(pname));
						strncpy(pname, buffer + 10, sysparmlen - 3);
					}
					strcat(plibarr, pname);
				}
				else
				{
					fclose(stream);
                    free(token);
                    *sysparm = NULL;
                    return 0;
				}
			}
		}
		fclose(stream);
	}
	free(token);
	*sysparm = plibarr;
	return ROWS;
}
void getfname(char *fname, const char *iplds, const char *dsn)
{
	char fnamepre[48];
	char fnamepost[8];
	memset(fnamepre, '\0', sizeof(fnamepre));
	strcat(fnamepre, "//'");
	strcat(fnamepre, iplds);

	memset(fnamepost, '\0', sizeof(fnamepost));
	strcat(fnamepost, "(");
	strcat(fnamepost, dsn);
	strcat(fnamepost, ")'");
	strcat(fname, fnamepre);
	strcat(fname, fnamepost);
}
int parmlibarr(char **plibarra, char *finame)
{
	FILE *stream;
	char buffer[NUM_ALPHA];
	buffer[NUM_ALPHA - 1] = '\0';
	char delim[] = "PARMLIB";
	char pname[45];
	char vname[7];
	int ROWS = 0;
	int COLS = 53;
	char *plibarr = NULL;
	char *token = malloc(sizeof(char) * strlen(delim));
    if (token == NULL)
    {
        *plibarra = NULL;
        return 0;
    }

	if ((stream = fopen(finame, "r")) != NULL)
	{
		while ((fgets(buffer, NUM_ALPHA, stream)) != NULL)
		{

			memset(token, '\0', sizeof(token));
			strncpy(token, buffer, strlen(delim));
			if (strcmp(token, delim) == 0)
			{
				memset(pname, '\0', sizeof(pname));
				memset(vname, '\0', sizeof(vname));
				ROWS = ROWS + 1;
				char *tmp = realloc(plibarr, sizeof(char) * (ROWS*COLS));
				if (tmp)
				{
					plibarr = tmp;
					memset(plibarr + ((ROWS - 1)*COLS), '\0', sizeof(char)* COLS);
					if (strlen(buffer) < 54)
					{
						strncpy(pname, buffer + 9, strlen(buffer) - 10);
						memset(pname + strlen(pname), ' ', 44 - strlen(pname));
					}
					else
					{
						strncpy(pname, buffer + 9, 44);
					}
					if (strlen(buffer) >= 55)
					{
						strncpy(vname, buffer + 54, 6);
					}
					strcat(plibarr, pname);
					strcat(plibarr, ":");
					strcat(plibarr, vname);
					strcat(plibarr, "-");
				}
				else
				{
                    fclose(stream);
                    free(token);
                    *plibarra = NULL;
                    return 0;
				}
			}

		}
		fclose(stream);
	}

	free(token);
	*plibarra = plibarr;
	return ROWS;
}

static void josnArrayLOADxx(jsonPrinter *printer, char *ipldsn, char *floadxx, char *fname)
{
	FILE *stream;
	int num;
	char buffer[NUM_ALPHA + 1];
	buffer[NUM_ALPHA] = '\0';

    jsonStartArray(printer, "LOADxx");
    jsonStartObject(printer, NULL);
	jsonAddString(printer, "Dataset", ipldsn);
	jsonAddString(printer, "Member", floadxx);

    //Create a Json array for contents
    jsonStartArray(printer, "Lines");
    //jsonStartObject(printer, NULL);
	//FILE processing
	if ((stream = fopen(fname, "r")) != NULL)
	{
		while ((fgets(buffer, NUM_ALPHA, stream)) != NULL)
		{
			jsonAddString(printer, NULL, buffer);
		}
        fclose(stream);
	}
    //jsonEndObject(printer);
	jsonEndArray(printer);

    jsonEndObject(printer);
    jsonEndArray(printer);
}

int printIPLData(jsonPrinter *printer)
{
	int status=0;
	int * __ptr32 ptrecvtipa;
	char eye_catcher[5];
	char LOADxx[3];
	char ipldsn[45];
	char floadxx[7];
	char *fname;
	char *ptr;
	char delim[] = " ";
	fname = (char *)malloc(57);
    if (fname == NULL)
	{
		return 0;
	}
	memset(fname, '\0', sizeof(fname));
	// parm lib array
	int ROWS = 1;
	int PCOLS = 53;
	int SCOLS = 53;
	char *plibarray = (char *)malloc(sizeof(char) * PCOLS);
	if (plibarray == NULL)
	{
		return 0;
	}

	char *syspamrarray = (char *)malloc(sizeof(char) * SCOLS);
	if (syspamrarray == NULL)
	{
        free(fname);
        free(plibarray);
		return 0;
	}
	
	// Initialize everything to NULL 
	memset(eye_catcher, '\0', sizeof(eye_catcher));
	memset(LOADxx, '\0', sizeof(LOADxx));
	memset(ipldsn, '\0', sizeof(ipldsn));
	memset(floadxx, '\0', sizeof(floadxx));
    
    // GET the ptrecvtipa
	ptrecvtipa = getECVTIPAs(); 
	memcpy(eye_catcher, ptrecvtipa, sizeof(eye_catcher) - 1);
	memcpy(LOADxx, (ptrecvtipa + (0x14 / 4)), sizeof(LOADxx) - 1);
	memcpy(ipldsn, (ptrecvtipa + (0x30 / 4)), sizeof(ipldsn) - 1);
	strcpy(floadxx, "LOAD");
	memcpy(floadxx + 4, LOADxx, sizeof(LOADxx) - 1);
	getfname(fname, ipldsn, floadxx);

    //Add LOADxx to JSON response
	josnArrayLOADxx(printer, ipldsn, floadxx, fname);

	ROWS = parmlibarr(&plibarray, fname);

    if (plibarray == NULL || ROWS == 0 )
	{
		return 0;
	}

	ROWS = getsysparm(&syspamrarray, fname);

	if (syspamrarray == NULL || ROWS == 0 )
	{
		return 0;
	}

	status=josnArrayIEASYSxx(printer, plibarray, syspamrarray);
	status=josnArrayPROGxx(printer, plibarray, syspamrarray);
	status=josnArrayBPXPRM(printer,plibarray, syspamrarray);
    
	free(fname);
	free(plibarray);
	free(syspamrarray);
	return 0;
}