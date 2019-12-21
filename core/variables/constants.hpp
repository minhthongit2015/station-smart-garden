
#pragma once
#ifndef BEYOND_GARDEN_CONSTANTS_H
#define BEYOND_GARDEN_CONSTANTS_H

#define POST "POST"
#define GET "GET"
#define PUT "PUT"
#define PATCH "PATCH"
#define DELETE "DELETE"

#define ApiEndpoint "/api/v1"
#define GardenEndpoint ApiEndpoint "/garden"
#define GardensEndpoint GardenEndpoint "/gardens"
#define StationsEndPoint GardenEndpoint "/stations"
#define RecordsEndpoint GardenEndpoint "/records"
#define VerifyStationEndpoint StationsEndPoint "/verify"

#endif
