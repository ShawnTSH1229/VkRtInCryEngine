// Copyright 2017-2021 Crytek GmbH / Crytek Group. All rights reserved.

using System;

namespace CryEngine
{
	/// <summary>
	/// Exposes the specified signal to Schematyc
	/// </summary>
	[AttributeUsage(AttributeTargets.Delegate)]
	public sealed class SchematycSignalAttribute : Attribute
	{
		/// <summary>
		/// Exposes the specified signal to Schematyc
		/// </summary>
		public SchematycSignalAttribute() { }
	}
}