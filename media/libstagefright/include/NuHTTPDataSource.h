#ifndef NU_HTTP_DATA_SOURCE_H_

#define NU_HTTP_DATA_SOURCE_H_

#include <media/stagefright/DataSource.h>
#include <utils/String8.h>
#include <utils/threads.h>

#include "HTTPStream.h"

namespace android {

struct NuHTTPDataSource : public DataSource {
    NuHTTPDataSource();

    status_t connect(
            const char *uri,
            const KeyedVector<String8, String8> *headers = NULL,
            off_t offset = 0);

    void disconnect();

    virtual status_t initCheck() const;

    virtual ssize_t readAt(off_t offset, void *data, size_t size);
    virtual status_t getSize(off_t *size);
    virtual uint32_t flags();

    virtual DecryptHandle* DrmInitialization(DrmManagerClient *client);
    virtual void getDrmInfo(DecryptHandle **handle, DrmManagerClient **client);

protected:
    virtual ~NuHTTPDataSource();

private:
    enum State {
        DISCONNECTED,
        CONNECTING,
        CONNECTED
    };

    Mutex mLock;

    State mState;

    String8 mHost;
    unsigned mPort;
    String8 mPath;
    String8 mHeaders;
    String8 mUri;

    HTTPStream mHTTP;
    off_t mOffset;
    off_t mContentLength;
    bool mContentLengthValid;

    DecryptHandle *mDecryptHandle;
    DrmManagerClient *mDrmManagerClient;

    status_t connect(
            const char *uri, const String8 &headers, off_t offset);

    status_t connect(
            const char *host, unsigned port, const char *path,
            const String8 &headers,
            off_t offset);

    void applyTimeoutResponse();

    static void MakeFullHeaders(
            const KeyedVector<String8, String8> *overrides,
            String8 *headers);

    NuHTTPDataSource(const NuHTTPDataSource &);
    NuHTTPDataSource &operator=(const NuHTTPDataSource &);
};

}  // namespace android

#endif  // NU_HTTP_DATA_SOURCE_H_
