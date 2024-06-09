// Copyright 2018-2021 Crytek GmbH / Crytek Group. All rights reserved.

#pragma once

#include "IPlatformBase.h"
#include <CryCore/Platform/IPlatformOS.h>

namespace Cry
{
	namespace GamePlatform
	{
		//! Interface of a game platform service (Steam, PSN, Discord etc.)
		struct IService : public IBase
		{
			enum class EProductType
			{
				Consumable,
				DLC,

				Count // Internal use
			};

			struct SStoreItem
			{
				ApplicationIdentifier id;
				string name;
				EProductType type = EProductType::Count;

				void Serialize(Serialization::IArchive& archive)
				{
					archive(id, "id");
					archive(name, "name");
					archive(type, "type");
				}

				bool operator==(const SStoreItem& rhs) const
				{
					return id == rhs.id;
				}
			};

			struct SStoreItemDetails
			{
				ApplicationIdentifier id;
				string name;
				EProductType type;
				string description;
				bool isPurchasable;
				string currencyCode;
				double price;        //! Current price (i.e. listing price after discounts)
				double listPrice;    //! Original listing price
				string displayPrice;
				string displayListPrice;
				uint32 consumableQuantity;

				void Serialize(Serialization::IArchive& archive)
				{
					archive(id, "id");
					archive(name, "name");
					archive(type, "type");
					archive(description, "description");
					archive(isPurchasable, "purchasable");
					archive(currencyCode, "currencycode");
					archive(price, "price");
					archive(listPrice, "listprice");
					archive(displayPrice, "displayprice");
					archive(displayListPrice, "displaylistprice");
					archive(consumableQuantity, "quantity");
				}

				bool operator==(const SStoreItemDetails& rhs) const
				{
					return id == rhs.id;
				}
			};

			struct SInventoryItem
			{
				InventoryItemIdentifier id;
				ApplicationIdentifier productId; //! Associated product ID. Can be empty.
				uint32 quantity;
			};

			enum class EPermission
			{
				Communication,
				Multiplayer,
				ViewProfiles,
				WebBrowser,
				UserGeneratedContent,
				MultiplayerSubscription,
				CrossNetworkPlay,
			};

			enum class EPrivacyPermission
			{
				VoiceCommunication,
				TextCommunication,
				PlayMultiplayer
			};

			enum class ERelationship
			{
				Default, //! Default relationship
				Friend,  //! Hint a friend relationship by a non-platform setting
			};

			enum class EPermissionResult
			{
				NotApplicable, //! The platform neither grants nor denies the permission
				Pending,       //! The platform has not returned the permission result yet
				Granted,       //! The platform grants the permission explicitly
				Denied,        //! The platform denies the permission explicitly
			};

			//! Listener to check for general game platform events
			//! See IService::AddListener and RemoveListener
			struct IListener
			{
				//! Called when the in-game platform layer is opened (usually by the user)
				virtual void OnOverlayActivated(const ServiceIdentifier& serviceId, bool active) {}
				//! Called when an avatar requested using RequestUserInformation has become available
				virtual void OnAvatarImageLoaded(const AccountIdentifier& accountId) {}
				//! Called when the service is about to shut down
				virtual void OnShutdown(const ServiceIdentifier& serviceId) {}
				//! Called when an account has been added
				virtual void OnAccountAdded(IAccount& account) {}
				//! Called right before removing an account
				virtual void OnAccountRemoved(IAccount& account) {}
				//! Called when the persona state was updated
				virtual void OnPersonaStateChanged(const IAccount& account, CEnumFlags<EPersonaChangeFlags> changeFlags) {}
				//! Called when a steam auth ticket request received a response
				virtual void OnGetSteamAuthTicketResponse(bool success, uint32 authTicket) {}
				//! Called when a token request received a response
				virtual void OnAuthTokenReceived(bool success, const char* szToken) {}
				//! Called when the connection status to the platform service changed
				virtual void OnNetworkStatusChanged(const EConnectionStatus& connectionStatus) {}
				//! Called when a microtransaction authorization has request had received a response
				virtual void OnMicroTransactionAuthorizationResponse(const TransactionIdentifier& orderId, bool authorized) {}
				//! Called when the licenses owned by the active account have changed
				virtual void OnLicensesChanged(const DynArray<InventoryItemIdentifier>& newLicenses) {}
				//! Called when the catalog has been queried
				virtual void OnCatalogReceived(bool success, const DynArray<SStoreItem>& items, bool remainingItemsInCatalog) {}
				//! Called when details for catalog items have been queried
				virtual void OnCatalogItemsReceived(bool success, const DynArray<SStoreItemDetails>& items) {}
				//! Called when the inventory has been queried
				virtual void OnInventoryReceived(bool success, const DynArray<SInventoryItem>& items) {}
				//! Called when the inventory has been consumed
				virtual void OnInventoryItemConsumed(bool success, const InventoryItemIdentifier& id, uint32 quantity) {}
				//! Called when a platform service error occurred
				virtual void OnPlatformServiceError(const AccountIdentifier& accountId, int32 errorCode) {}
				//! Called when the state of a user permission has been changed
				virtual void OnUserPermissionChanged(const AccountIdentifier& accountId, EPermission permission, bool hasPermission) {}
				//! Called when the name of an account has changed
				virtual void OnAccountNameChanged(const AccountIdentifier& accountId, const char* szName) {}
				//! Called when the local account's login state changed
				virtual void OnLogInStateChanged(const AccountIdentifier& accountId, bool isLoggedIn) {}
				//! Called when the value of an environment variable has changed
				virtual void OnEnvironmentVariableChanged(const char* szVarName, const char* szNewValue) {}

			protected:
				~IListener() {}
			};

			enum class ECatalogSortOrder
			{
				AllPurchaseCountDaily,    //! Number of free and paid purchases on the most recent day
				DigitalReleaseDate,       //! Date on which the item was made available for download
				PaidPurchaseCountAllTime, //! Number of overall paid purchases
				PaidPurchaseCountDaily,   //! Number of paid purchases on the most recent day
				PhysicalReleaseDate,      //! Date on which the item was made available in retail stores, falls back to DigitalReleaseDate
				UserRating,               //! Average user rating

				Count //! Internal use
			};

			virtual ~IService() {}

			//! Adds a service event listener
			virtual void AddListener(IListener& listener, const char* szName) = 0;
			//! Removes a service event listener
			virtual void RemoveListener(IListener& listener) = 0;

			//! Called by core platform plugin before it's going to be unloaded
			virtual void Shutdown() = 0;

			//! Gets the unique identifier of this service
			virtual ServiceIdentifier GetServiceIdentifier() const = 0;

			//! Returns the platform identifier of the build the player is running, usually the trivial version of the application version
			virtual int GetBuildIdentifier() const = 0;

			//! Try to set the associated platform specific identifier from the given string representation
			//! \param out The identifier to set
			//! \param in The string representation of the identifier value
			virtual bool SetIdentifierFromString(AccountIdentifier& out, const string& in) const = 0;
			virtual bool SetIdentifierFromString(LobbyIdentifier& out, const string& in) const = 0;
			virtual bool SetIdentifierFromString(ApplicationIdentifier& out, const string& in) const = 0;
			virtual bool SetIdentifierFromString(InventoryItemIdentifier& out, const string& in) const = 0;
			virtual bool SetIdentifierFromString(TransactionIdentifier& out, const string& in) const = 0;
			virtual bool SetIdentifierFromString(LeaderboardIdentifier& out, const string& in) const = 0;

			//! Checks if the local user owns the specified identifier
			//! \param id The platform-specific identifier for the application or DLC
			virtual bool OwnsApplication(ApplicationIdentifier id) const = 0;

			//! Gets the platform-specific identifier for the running application
			virtual ApplicationIdentifier GetApplicationIdentifier() const = 0;

			//! Gets an IAccount representation of the local user, useful for getting local information such as user name
			virtual IAccount* GetLocalAccount() const = 0;
			//! Gets local user's friend accounts
			virtual const DynArray<IAccount*>& GetFriendAccounts() const = 0;
			//! Gets local user's blocked accounts
			virtual const DynArray<IAccount*>& GetBlockedAccounts() const = 0;
			//! Gets local user's muted accounts
			virtual const DynArray<IAccount*>& GetMutedAccounts() const = 0;

			//! Allows retrieval of platform-specific information that can't be easily added to the IService interface without bloating it
			//! \param szVarName The variable name
			//! \param valueOut This is where the variable value will be stored (if any)
			//! \retval true Value was found and stored in output variable
			//! \retval false Unknown variable name
			virtual bool GetEnvironmentValue(const char* szVarName, string& valueOut) const = 0;

			//! Manually refresh friend account list of the local account
			virtual void RefreshFriendAccounts() = 0;
			//! Manually refresh blocked account list of the local account
			virtual void RefreshBlockedAccounts() = 0;
			//! Manually refresh muted account list of the local account
			virtual void RefreshMutedAccounts() = 0;
			//! Manually refresh permissions of the local account
			virtual void RefreshPermissions() = 0;
			//! Manually refresh privacy permissions with specified target accounts of the local account
			//! \param pTargetAccountIds The target account id's. Optional. If null, will refresh for all accounts added to local session
			virtual void RefreshPrivacyPermissions(const DynArray<AccountIdentifier>* pTargetAccountIds) = 0;

			//! Gets an IAccount representation of another user by account id
			virtual IAccount* GetAccountById(const AccountIdentifier& accountId) const = 0;
			//! Adds the account into a lobby or match context for interaction relevant updates (GetPrivacyPermission)
			virtual void AddAccountToLocalSession(const AccountIdentifier& accountId) = 0;
			//! Removes the account from a lobby or match context
			virtual void RemoveAccountFromLocalSession(const AccountIdentifier& accountId) = 0;

			//! Checks if the local user has the other user in their friends list for this service
			virtual bool IsFriendWith(const AccountIdentifier& otherAccountId) const = 0;
			//! Opens a known dialog targeted at a specific user id via the platform's overlay
			virtual bool OpenDialogWithTargetUser(EUserTargetedDialog dialog, const AccountIdentifier& otherAccountId) const = 0;

			//! Opens a known dialog via the platform's overlay
			virtual bool OpenDialog(EDialog dialog) const = 0;
			//! Opens a browser window via the platform's overlay
			virtual bool OpenBrowser(const char* szURL) const = 0;
			//! Checks whether we are able to open the overlay used for purchasing assets
			virtual bool CanOpenPurchaseOverlay() const = 0;
			//! Open the platform's purchase overlay with the provided product ID
			virtual bool OpenPurchaseOverlay(const ApplicationIdentifier& productId) = 0;

			//! Get the content of the catalog. Invokes IListener::OnCatalogReceived once done
			virtual bool BrowseCatalog(ECatalogSortOrder sortOrder) const = 0;
			//! Get detailed information about a catalog item. Invokes IListener::OnCatalogItemsReceived once done
			virtual bool QueryCatalogItemDetails(const SStoreItem& item) const = 0;
			//! Get detailed information about a catalog item. Invokes IListener::OnCatalogItemsReceived once done
			virtual bool QueryCatalogItemDetails(const ApplicationIdentifier& productId) const = 0;
			//! Get detailed information about multiple catalog items. Invokes IListener::OnCatalogItemsReceived once done
			virtual bool QueryCatalogItemDetails(const DynArray<SStoreItem>& items) const = 0;
			//! Get detailed information about multiple catalog items. Invokes IListener::OnCatalogItemsReceived once done
			virtual bool QueryCatalogItemDetails(const DynArray<ApplicationIdentifier>& productIds) const = 0;

			//! Query the content of the platform inventory. Invokes IListener::OnInventoryReceived once done
			virtual bool QueryInventory() const = 0;
			//! Consume an item from the platform inventory. Invokes IListener::OnInventoryItemConsumed once done
			virtual bool ConsumeInventoryItem(const InventoryItemIdentifier& id, uint32 quantity = 1) const = 0;

			//! Check if information about a user (e.g. personal name, avatar...) is available, otherwise download it.
			//! \note It is recommended to limit requests for bulky data (like avatars) to a minimum as some platforms may have bandwidth or other limitations.
			//! \retval true Information is not yet available and listeners will be notified once retrieved.
			//! \retval false Information is already available and there's no need for a download.
			virtual bool RequestUserInformation(const AccountIdentifier& accountId, UserInformationMask info) = 0;

			//! CHeck if there is an active connection to the service's backend
			virtual bool IsLoggedIn() const = 0;

			//! Check if a user has permission to perform certain actions on the platform
			virtual EPermissionResult GetPermission(EPermission permission) const = 0;

			//! Check if the user's privacy settings grant permission to perform certain actions with the target user
			virtual EPermissionResult GetPrivacyPermission(const AccountIdentifier& targetAccountId, EPrivacyPermission permission, ERelationship targetRelationship = ERelationship::Default) const = 0;

			//! Get a list of all applications that the user owns and that relate to this game (e.g. DLCs)
			virtual const DynArray<InventoryItemIdentifier>& GetLicenses() const = 0;

			//! Forces the service to verify the game content on next launch
			virtual void MarkContentCorrupt(bool missingFilesOnly) const = 0;

			//! Set whether the user is currently considered to be in real time multiplayer
			virtual void SetIsInMultiplayer(bool isInMultiplayer) = 0;

			//! Filter out of given text words that are not allowed on the platform
			virtual bool FilterText(const char* szText, std::function<void(const char* szFiltered)> responseCallback) const = 0;
		};
	}
}
