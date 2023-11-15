using Steamworks;


namespace Steam
{
    class SteamScript
    {

        public static void SendQueryUGCRequest<T>(UGCQueryHandle_t handle, CallResult<T>.APIDispatchDelegate func)
        {
            SteamAPICall_t apiCall = SteamUGC.SendQueryUGCRequest(handle);
            var callResult = CallResult<T>.Create();
            callResult.Set(apiCall, func);

            RunCallbacks(callResult);
        }

        public static void RequestUGCDetails<T>(PublishedFileId_t nPublishedFileID, CallResult<T>.APIDispatchDelegate func)
        {
            SteamAPICall_t apiCall = SteamUGC.RequestUGCDetails(nPublishedFileID, 0);
            var callResult = CallResult<T>.Create();
            callResult.Set(apiCall, func);
            RunCallbacks(callResult);
        }

        public static void RunCallbacks<T>(CallResult<T> callResult)
        {
            while (callResult.IsActive())
            {
                SteamAPI.RunCallbacks();
                System.Threading.Thread.Sleep(1000);
            }
        }
    }
}