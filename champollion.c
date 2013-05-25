#include <stdio.h>
#include <curl/curl.h>

#define FALSE 0
#define TRUE 1

typedef enum {NONE, GPS_LON, GPS_LAT, GPS_ALT, TEMP, ACC} State;
State state = NONE;

char lon[50];
char lat[50];
char alt[50];

char *lon_ptr;
char *lat_ptr;
char *alt_ptr;

int comma_found;
int separator_found;


void reset()
{
	lon_ptr = (char*) &lon;
	lat_ptr = (char*) &lat;
	alt_ptr = (char*) &alt;
}

void addChar(char c)
{
	switch(state)
	{
		case GPS_LON:
			*lon_ptr++ = c;
			break;
		case GPS_LAT:
			*lat_ptr++ = c;
			break;
		case GPS_ALT:
			*alt_ptr++ = c;
			break;
	}
}

void SendGPS(char *lon, char *lat, char *alt)
{
	*lon_ptr = '\0';
	*lat_ptr = '\0';
	*alt_ptr = '\0';
	
	CURL * myHandle;
	CURLcode result;
	myHandle = curl_easy_init ( ) ;
	
	char buffer[300];
	sprintf(buffer, "http://127.0.0.1/saveGpsRecord/%s/%s/%s", lon, lat, alt);
	curl_easy_setopt(myHandle, CURLOPT_URL, buffer);
	result = curl_easy_perform( myHandle );
	curl_easy_cleanup( myHandle ); 
}

int main()
{	
	curl_global_init( CURL_GLOBAL_ALL );
	state = NONE;
	
	while(1)
	{
		char c;
		scanf("%c",&c);
		
		if(c >= 'A' && c <= 'J')
		{
			char num = (char) (c + '0' - 'A');
			addChar(num);
			comma_found = FALSE;
			separator_found = FALSE;
			printf("%c", num);
		}
		
		switch(c)
		{
			case 'K':
				printf("GPS:");
				state = GPS_LON;
				reset();		
				break;
/*			case 'L':
				printf("Temp:");
				state = TEMP;
				reset();
				break;
			case 'M':
				printf("Acc:");
				state = ACC;
				reset();
				break;*/
			case 'R':
				if(comma_found) break;
				comma_found = TRUE;
				printf(".");
				addChar('.');
				break;
			case 'S':
				if(separator_found) break;
				separator_found = TRUE;
				printf("!");
				switch(state)
				{
					case GPS_LON: state = GPS_LAT; break;
					case GPS_LAT: state = GPS_ALT; break;
				}
				break;
			case 'T':
				printf("STOP\n");
				if(state == GPS_ALT)
				{
					SendGPS((char*) &lon, (char*) &lat, (char*) &alt);
					state = NONE;
					reset;
				}
				break;
		}
		
	}
	
}
