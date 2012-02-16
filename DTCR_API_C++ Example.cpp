// DTCR_API_C++ Example.cpp : Defines the entry point for the console application.

#include "stdafx.h"

using namespace std;

int _tmain(int argc, _TCHAR* argv[])
{	
	const int BUFLEN = 3000; //the suggested max length of error messages was 2000
	const int MAX_MSG_LENGTH = 40;
	string EDS, LDS, ADS, AirTemp, EquipID, line;
	float Duration, Onset, MaxHS, MaxTO, MaxLL;		
	int FunctionChoice, ReturnCode;

	//variables changed by ComputeTransformerRating2/other functions
	float RatingMVA = 0, 
		  RatingAMPS = 0, 
		  MaxIncurredHS = 0, 
		  MaxIncurredTO = 0, 
		  MaxLIL = 0, 
		  MaxIBO = 0, 
		  MaxH = 0, 
		  MaxX = 0, 
		  MaxY = 0;
	char EmsgsDefineElement[BUFLEN] = {'\0'};
	char EmsgsComputeTransformerRating2[BUFLEN] = {'\0'};
	char LimitingFactor[BUFLEN] = {'\0'};
	//----------------------------------------------

	cout<<"Element Definition Strings: \n";
	ifstream inputEDS("EDS.txt");
	while(getline(inputEDS, line)){
		EDS += line + "\n";
	}
	ifstream inputLDS("LDS.txt");
	cout<<"Load Definition String: \n";
	while(getline(inputLDS, line)){
		LDS += line + "\n";
	}
	//cout<<LDS;
	ifstream inputADS("ADS.txt");
	cout<<"Ambient Definition String: \n";
	while(getline(inputADS, line)){
		ADS += line + "\n";
	}
	//cout<<ADS;
	cout<<"EquipID: ";
	cin>>EquipID;
	cout<<"Duration (hours): ";
	cin>>Duration;
	cout<<"Onset (hours): ";
	cin>>Onset;
	cout<<"Air Temp (c): ";
	cin>>AirTemp;
	cout<<"Max HS (c): ";
	cin>>MaxHS;
	cout<<"Max TO (c): ";
	cin>>MaxTO;
	cout<<"Max LL (%): ";
	cin>>MaxLL;
	cout<<"ComputeTransformerRating2 (1) | DefineElement (2): ";
	cin>>FunctionChoice;

	if(FunctionChoice == 1){ //DefineElement and ComputeTransformerRating2
		//declares function parameter list for error checking (must use __stdcall instead of __cdecl, or %esp is trashed apparently)
		typedef int (__stdcall *MYPROC)(const char*, const char*, int, char*); 
		typedef int (__stdcall *MYPROC2)(const char*,float,float,const char*,const char*,const char*,float,float,float,int,int,float*,float*,float*,float*,float*,float*,char*,char*,float*,float*,float*); 
		typedef int (__stdcall *MYPROC3)(bool, const char*);	
		typedef int (__stdcall *MYPROC4)(bool, const char*);

		HINSTANCE hinstLib; 
		MYPROC ProcAdd;
		MYPROC2 ProcAdd2; 
		MYPROC3 ProcAdd3;
		MYPROC4 ProcAdd4;
	
		// Get a handle to the DLL module.
		hinstLib = LoadLibrary(TEXT("DTCR_API.dll"));  
		// If the handle is valid, try to get the function address.
		if (hinstLib != NULL) 
		{ 
			ProcAdd = (MYPROC) GetProcAddress(hinstLib, "DefineElement");
			ProcAdd2 = (MYPROC2) GetProcAddress(hinstLib, "ComputeTransformerRating2"); 
			ProcAdd3 = (MYPROC3) GetProcAddress(hinstLib, "EnableDebugFile");
			ProcAdd4 = (MYPROC4) GetProcAddress(hinstLib, "EnableErrorFile");

			// If the function address is valid, call the function.
			if (NULL != ProcAdd && NULL != ProcAdd2) 
			{ 		
				//call EnableDebugFile
				//ProcAdd3(true, "C:\\debug_temp.txt");
				//call EnableErrorFile
				//ProcAdd4(true, "C:\\error_temp.txt");
				//call DefineElement
				ReturnCode = (ProcAdd)(EquipID.c_str(), EDS.c_str(), 2000, EmsgsDefineElement);
				//call ComputeTransformerRating2
				ReturnCode = (ProcAdd2)(EquipID.c_str(), Duration, Onset, 
					"FLAT_SPIKE", LDS.c_str(), ADS.c_str(), MaxHS, MaxTO, MaxLL, 
					2000, 40, &RatingMVA, &RatingAMPS, &MaxIncurredHS, &MaxIncurredTO, &MaxLIL, &MaxIBO, EmsgsComputeTransformerRating2, LimitingFactor, &MaxH, &MaxX, &MaxY);	
				
				//cout<<"Errors (DefineElement): \n"<<EmsgsDefineElement;
				//cout<<"\nErrors (ComputeTransformerRating2): \n"<<EmsgsComputeTransformerRating2;
			}
			// Free the DLL module.
			FreeLibrary(hinstLib); 
		} 
	}else if(FunctionChoice == 2){ //DefineElement
		//declares function parameter list for error checking
		typedef int (__stdcall *MYPROC)(const char*, const char*, int, char*); 
		
		HINSTANCE hinstLib; 
		MYPROC ProcAdd; 
	 
		// Get a handle to the DLL module.
		hinstLib = LoadLibrary(TEXT("DTCR_API.dll")); 
	 
		// If the handle is valid, try to get the function address.
		if (hinstLib != NULL) 
		{ 
			ProcAdd = (MYPROC) GetProcAddress(hinstLib, "DefineElement"); 
	 
			// If the function address is valid, call the function.
			if (NULL != ProcAdd) 
			{ 			
				ReturnCode = (ProcAdd)(EquipID.c_str(), EDS.c_str(), 2000, EmsgsDefineElement);
			}
			// Free the DLL module.
			FreeLibrary(hinstLib); 
		} 
	}

	switch (ReturnCode){
		case 0:
			cout<<ReturnCode << " - Success\n";
			if(FunctionChoice == 1){
				cout<<"RatingMVA: "<<RatingMVA;
				cout<<"\nRatingAMPS: "<<RatingAMPS;
				cout<<"\nMaxH: "<<MaxIncurredHS;
				cout<<"\nMaxX: "<<MaxIncurredTO;
				cout<<"\nMaxY: "<<MaxIBO;
				cout<<"\nMaxLIL: "<<MaxLIL;
			}
			break;
		case 1:
			cout<<ReturnCode << " - A required TIME function has not yet been implemented";
			break;
		case 2:
			cout<<ReturnCode << " - TIME was unable to create a pop-up message box";
			break;
		case 3:
			cout<<ReturnCode << " - The element definition string was not valid";
			break;
		case 4:
			cout<<ReturnCode << " - An unspecified program exception occured while instantiating a transformer";
			break;
		case 5:
			cout<<ReturnCode << " - Future use";
			break;
		case 6:
			cout<<ReturnCode << " - The requested EQUIP_ID has not been defined";
			break;
		case 7:
			cout<<ReturnCode << " - An unspecified program exception has occurred while computing a rating";
			break;
		case 8:
			cout<<ReturnCode << " - At least one data channel is missing or improperly defined";
			break;
		case 9:
			cout<<ReturnCode << " - Unable to compute rating because of an initialization error";
			break;
		case 10:
			cout<<ReturnCode << " - Unable to run preload cycle";
			break;
		case 11:
			cout<<ReturnCode << " - Unable to compute rating";
			break;
		case 12:
			cout<<ReturnCode << " - For now, ratings can only be computed on power transformers";
			break;
		case 13:
			cout<<ReturnCode << " - Pre-contingency load too high (constraints violated at the start of the contingency)";
			break;
		case 14:
			cout<<ReturnCode << " - Post-contingency load too high (constraints violated after the contingency)";
			break;
		case 15:
			cout<<ReturnCode << " - The load definiton string was not a recognizable allowed type";
			break;
		default:
			cout<<ReturnCode<<" ERROR OCCURED";
	}

	cout<<"\n\n";
	system("pause");
	return 0;
}

