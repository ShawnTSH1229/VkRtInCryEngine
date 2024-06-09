// Copyright 2017-2021 Crytek GmbH / Crytek Group. All rights reserved.

using System;

namespace CryEngine
{
	/// <summary>
	/// Exposes the specified method to Schematyc
	/// </summary>
	[AttributeUsage(AttributeTargets.Method)]
	public sealed class SchematycMethodAttribute : Attribute
	{
		/// <summary>
		/// Exposes the specified method to Schematyc
		/// </summary>
		public SchematycMethodAttribute() { }
	}
}