// Copyright 2017-2021 Crytek GmbH / Crytek Group. All rights reserved.



SERIALIZATION_ENUM_BEGIN_NESTED(SSubstanceOutput, ESubstanceOutputResolution, "Output Resolution")
SERIALIZATION_ENUM(SSubstanceOutput::ESubstanceOutputResolution::Original, "original", "Not Changed")
SERIALIZATION_ENUM(SSubstanceOutput::ESubstanceOutputResolution::Half, "half", "Half")
SERIALIZATION_ENUM(SSubstanceOutput::ESubstanceOutputResolution::Quarter, "quarter", "Quarter")
SERIALIZATION_ENUM(SSubstanceOutput::ESubstanceOutputResolution::Eighth, "eighth", "Eighth")
SERIALIZATION_ENUM_END()
