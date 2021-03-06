#if defined _serversys_core_included
  #endinput
#endif
#define _serversys_core_included

typeset Sys_ChatCommand_CB {
	function void(int client);
	function void(int client, const char[] command);
	function void(int client, const char[] command, const char[] args);
}

typedef Sys_TxnSuccess = function void(any data, int numQueries, Handle[] results, any[] queryData);
typedef Sys_TxnFailure = function void(any data, int numQueries, const char[] error, int failIndex, any[] queryData);

enum {
	NOBLOCK_TYPE_COLLISIONGROUP = 0,
	NOBLOCK_TYPE_SOLIDTYPE = 1
}

enum {
	SPAWNPROTECT_GODMODE = 0,
	SPAWNPROTECT_RESPAWN = 1
}

enum {
	HIDE_NORMAL = 0,
	HIDE_TEAM = 1
}

/**
* Called when the Server-Sys database is loaded.
*
* @param success		This value will be false if databases are disabled
*						for Server-Sys, or if the configuration key did not
*						exist in databases.cfg
*/
forward void OnDatabaseLoaded(bool success);

/**
* Called when Server-Sys has loaded and queried
* the database with the ServerID. This will only be
* called if databases are enabled (check the 'success'
* param from OnDatabaseLoaded to determine if it will)
*
* @param serverID		This value will be the verified (after MySQL)
*                       ServerID.
*/
forward void OnServerIDLoaded(int serverID);

/**
* Called after OnClientAuthorized, once the users'
* PlayerID is loaded. There might be a small delay.
*
* @param client         Client index
* @param playerID		This value will be the verified (after MySQL)
*                       ServerID.
*/
forward void OnPlayerIDLoaded(int client, int playerID);

/**
* Called after the Map ID is created or selected,
* which is done when OnMapStart is called. The call
* SHOULD in theory be prevented if another map change
* were to suddenly happen before the select query could
* be transacted, as this checks Sys_InMap() before firing.
*
* @param mapid          The ID of the map.
* @param mapname		The name of the map that the ID corresponds to.
*/
forward void OnMapIDLoaded(int mapid, const char[] mapname);




/**
* Reloads the core plugin's config file.
*
* @param map          Whether to load the maps config (if exists)
*                       or the default (core.cfg).
*/
native void Sys_ReloadConfiguration(bool map = true);

/**
* Checks if the core plugin has Map-Configs enabled.
*
* @return               True if enabled, false otherwise.
*/
native bool Sys_UseMapConfigs();

/**
* Checks if we are currently in between OnMapStart and
* OnMapEnd. Useful to check if things like the map's name
* and any other map-specific things are usable.
*
* @return               True if in between, false otherwise
*/
native bool Sys_InMap();

/**
* Checks if we are currently in between events round_start and
* round_end. Useful to check whether or not rounds will end.
*
* @return               True if in between, false otherwise
*                       Will also return false if Sys_InMap() is false.
*/
native bool Sys_InRound();

/**
* Checks if hide is enabled for a player.
*
* @param client         Client to check.
*/
native bool Sys_IsHideEnabled(int client);

/**
* Gets a PlayerID using client index
*
* @param client         Client to check.
* @return               PlayerID or -1 on failure
*/
native int Sys_GetPlayerID(int client);

/**
* Gets a client index by searching from PlayerID.
* This obviously only works if the client of this PID
* is in-game.
*
* @param playerid       PlayerID to search for.
* @return               Client index or 0.
*/
native int Sys_GetClientOfPlayerID(int playerid);

/**
* Gets the ServerID.
*
* @return               ServerID or -1 if not yet loaded (wait for OnServerIDLoaded)
*/
native int Sys_GetServerID();

/**
* Gets the name of the current server. This is based off
* of the value defined in core.cfg
*
* @param buffer         Buffer to store name in.
* @param maxlen         Max length of buffer.
*/
native void Sys_GetServerName(char[] buffer, int maxlen);

/**
* Gets the IP of the current server. This is based off
* of the value defined in core.cfg
*
* @param buffer         Buffer to store IP in.
* @param maxlen         Max length of buffer.
*/
native void Sys_GetServerIP(char[] buffer, int maxlen);

/**
* Gets the MapID. This will return -1 if we are not in
* a map, or if the MapID has not been loaded yet.
*
* @return               MapID or -1
*/
native int Sys_GetMapID();

/**
* Gets a PlayerID using auth (GetSteamAccountID). This
* feature is unavailable if Sys_DB_Connected() is false,
* which also means if databases are disabled. This is also
* unavailable until after OnPlayerIDLoaded is ran for this
* user.
*
* @param auth           Account ID of player to check.
* @return               PlayerID or -1 on failure
*/
//native int Sys_GetPlayerIDFromAuth(int auth);


/**
* Registers a chat command to the specified callback,
* credit for this functionality is mostly due to
* Drixevel's work on SourceMod-Store.
*
* @param command        String of commands to check, seperated by space.
*                           (Ex: '/hey !hey .hey ;;hey');
* @param callback       Callback to call when command is ran.
*
* @return               Whether or not register was successful.
*/
native bool Sys_RegisterChatCommand(const char[] command, Sys_ChatCommand_CB callback);

/**
* Check if the DB is connected.
*
* @return               True or false. Will also return
*                       false is Sys_UseDatabase() is false.
*/
native bool Sys_DB_Connected();

/**
* Queries the main Server-Sys database and returns like normal SQL_TQuery.
* This function was made possible by the open source SourceMod Store.
* http://github.com/Drixevel/store
*
* @param callback		Callback to send results to.
* @param query			Query to execute.
* @param data			Data to pass through callback.
* @param prio			Database priority to give.
*/
native void Sys_DB_TQuery(SQLTCallback callback, const char[] query, any data = 0, DBPriority prio = DBPrio_Normal);


/**
* Queries the main Server-Sys database and returns like normal SQL_Query.
* Do not use this function for high-cost methods, as server locking up is
* possible.
*
* @param query		    Query to execute.
*
* @return               DBResultSet result
*/
native DBResultSet Sys_DB_Query(const char[] query);

/**
* This is the same as SQL_ExecuteTransaction but ran on our database
* handle. Callbacks are the same except they are not passed the
* database handle as is with SQLTxnSuccess and SQLTxnFailure.
*
* @param txn            Transaction to execute.
* @param onSuccess      Optional success callback.
* @param onFailure      Optional failure callback.
* @param data           Optional data to pass to callback.
* @param prio       Optional priority for the transaction.
*/
native void Sys_DB_ExecuteTransaction(Transaction txn, Sys_TxnSuccess onSuccess = INVALID_FUNCTION, Sys_TxnFailure onFailure = INVALID_FUNCTION, any data = 0, DBPriority prio = DBPrio_Normal);

/**
* Escapes a string like normal SQL_EscapeString.
* Your safebuffer's size MUST be EXACTLY:
* 2*(sizeof(original))+1
*
* @param original       The original char to escape
* @param maxlen         Max length of original buffer
* @param safebuffer     The buffer to store the escaped char into
* @param safemaxlen     Max length of the safe buffer
* @param byteswritten   Optional int to store amount of bytes written
*/
native void Sys_DB_EscapeString(char[] original, int maxlen, char[] safebuffer, int safemaxlen, int &byteswritten = 0);



public SharedPlugin __pl_serversys_core =
{
	name = "serversys",
	file = "serversys-core.smx",
#if defined REQUIRE_PLUGIN
	required = 1,
#else
	required = 0,
#endif
};

#if defined REQUIRE_PLUGIN
public __pl_serversys_core_SetNTVOptional(){
    MarkNativeAsOptional("Sys_ReloadConfiguration");
    MarkNativeAsOptional("Sys_RegisterChatCommand");
    MarkNativeAsOptional("Sys_IsHideEnabled");
    MarkNativeAsOptional("Sys_UseMapConfigs");
    MarkNativeAsOptional("Sys_InMap");
    MarkNativeAsOptional("Sys_InRound");
    MarkNativeAsOptional("Sys_GetPlayerID");
    MarkNativeAsOptional("Sys_GetClientOfPlayerID");
    MarkNativeAsOptional("Sys_GetServerID");
    MarkNativeAsOptional("Sys_GetServerName");
    MarkNativeAsOptional("Sys_GetServerIP");
    MarkNativeAsOptional("Sys_GetMapID");
    MarkNativeAsOptional("Sys_DB_TQuery");
    MarkNativeAsOptional("Sys_DB_Query");
    MarkNativeAsOptional("Sys_DB_EscapeString");
    MarkNativeAsOptional("Sys_DB_ExecuteTransaction");
    MarkNativeAsOptional("Sys_DB_Connected");
}
#endif
