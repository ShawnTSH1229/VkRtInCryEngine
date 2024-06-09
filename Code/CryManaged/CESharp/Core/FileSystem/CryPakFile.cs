// Copyright 2017-2021 Crytek GmbH / Crytek Group. All rights reserved.

using CryEngine.Common;

namespace CryEngine.FileSystem
{
	/// <summary>
	/// File handle for the native ICryPak system.
	/// </summary>
	public class CryPakFile
	{
		[SerializeValue]
		private readonly _CrySystem_cs_SWIGTYPE_p_FILE _nativeHandle;

		internal _CrySystem_cs_SWIGTYPE_p_FILE NativeHandle { get { return _nativeHandle; } }

		internal CryPakFile(_CrySystem_cs_SWIGTYPE_p_FILE handle)
		{
			_nativeHandle = handle;
		}
	}
}
