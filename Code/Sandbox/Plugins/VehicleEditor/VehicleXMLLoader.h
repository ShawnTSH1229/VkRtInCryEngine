// Copyright 2005-2021 Crytek GmbH / Crytek Group. All rights reserved.

#pragma once

struct IVehicleData;

IVehicleData* VehicleDataLoad(const char* definitionFile, const char* dataFile, bool bFillDefaults = true);
IVehicleData* VehicleDataLoad(const XmlNodeRef& definition, const char* dataFile, bool bFillDefaults = true);
IVehicleData* VehicleDataLoad(const XmlNodeRef& definition, const XmlNodeRef& data, bool bFillDefaults = true);
