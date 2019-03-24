/******************************************************************************
 *
 * ADOBE CONFIDENTIAL
 * ___________________
 *
 * Copyright 2014 Adobe Systems Incorporated
 * All Rights Reserved.
 *
 * NOTICE:  All information contained herein is, and remains the property of
 * Adobe Systems Incorporated and its suppliers, if any. The intellectual and
 * technical concepts contained herein are proprietary to Adobe Systems
 * Incorporated and its suppliers and are protected by trade secret or
 * copyright law. Dissemination of this information or reproduction of this
 * material is strictly forbidden unless prior written permission is obtained
 * from Adobe Systems Incorporated.
 ******************************************************************************/

#ifndef AdobePhotoAssetHeader
#define AdobePhotoAssetHeader

#import "AdobePhoto.h"

#import "AdobePhotoAssetRendition.h"
#import "AdobePhotoTypes.h"

@class AdobePhotoCollection;
@class AdobePhotoCatalog;
@class AdobePhotoAssetRevision;
@class AdobePhotoAsyncRequest;

/** Asset Type */
typedef NS_ENUM (NSInteger, AdobePhotoAssetType)
{
    /** Image type */
    AdobePhotoAssetTypeImage = 1,
};

/**
 * Keys to use to reference AdobePhotoAssetRenditions in the rendition dictionary.
 */
extern NSString *const AdobePhotoAssetRenditionImageFullSize;
extern NSString *const AdobePhotoAssetRenditionImagePanorama;
extern NSString *const AdobePhotoAssetRenditionImage2048;
extern NSString *const AdobePhotoAssetRenditionImage1280;
extern NSString *const AdobePhotoAssetRenditionImage640;
extern NSString *const AdobePhotoAssetRenditionImageThumbnail2x;

/**
 * Metadata extraction rules used when generating renditions.
 */
extern NSString *const AdobePhotoAssetRenditionMetadataExtractAll;
extern NSString *const AdobePhotoAssetRenditionMetadataExtractNone;
extern NSString *const AdobePhotoAssetRenditionMetadataExtractCaption;
extern NSString *const AdobePhotoAssetRenditionMetadataExtractGeometry;
extern NSString *const AdobePhotoAssetRenditionMetadataExtractLocation;
extern NSString *const AdobePhotoAssetRenditionMetadataExtractXMP;
extern NSString *const AdobePhotoAssetRenditionMetadataSuppressCaption;
extern NSString *const AdobePhotoAssetRenditionMetadataSuppressGeometry;
extern NSString *const AdobePhotoAssetRenditionMetadataSuppressLocation;
extern NSString *const AdobePhotoAssetRenditionMetadataSuppressXMP;

typedef NSDictionary<NSString *, AdobePhotoAssetRendition *> AdobePhotoAssetRenditionDictionary;
typedef NSArray<NSNumber *> AdobePhotoAssetRenditionTypes;

/**
 * A utility to help determine if an AdobePhoto is an AdobePhotoAsset.
 */
#define IsAdobePhotoAsset(item) ([item isKindOfClass:[AdobePhotoAsset class]])

/**
 * An asset is a single photo or video (when supported). Each asset has one or more revisions; a revision can contain
 * multiple files or renditions of the asset.
 *
 * An asset is comprised of either a master or proxy file. The master data is either a JPEG or camera raw file,
 * where as the proxy data is a DNG (Digital Negative). There is no guarantee that the asset has master or proxy data,
 * but a check of the masterDataExists or proxyDataExists will report that.
 *
 * When exporting the asset, we recommend that a AdobePhotoAssetRenditionTypeImageFullSize rendition be generated
 * (if not currently present) and downloaded, unless the consumer of the output can support camera raw or DNG.
 *
 */
@interface AdobePhotoAsset : AdobePhoto <NSCopying, NSCoding>

/**
 * The catalog the asset is in.
 */
@property (readonly, nonatomic, strong) AdobePhotoCatalog *catalog;

/**
 * The asset's content-type.
 */
@property (nonatomic, readonly, strong) NSString *contentType;

/**
 * This asset's master.
 */
@property (nonatomic, readonly, strong) NSURL *master;

/**
 * Whether the master data exists in the cloud. If the asset was uploaded by Lightroom Desktop, this will be set to NO.
 */
@property (nonatomic, readonly, assign) BOOL masterDataExists;

/**
 * The asset metadata.
 */
@property (nonatomic, readonly, strong) NSDictionary *metadata;

/**
 * The name of the item.
 */
@property (nonatomic, readonly, strong) NSString *name;

/**
 * The asset's order (only valid in a collection).
 */
@property (nonatomic, strong) NSString *order;

/**
 * This asset's proxy.
 */
@property (nonatomic, readonly, strong) NSURL *proxy;

/**
 * Whether the proxy data exists in the cloud. If the asset was uploaded by Lightroom Desktop, this will be set to YES.
 */
@property (nonatomic, readonly, assign) BOOL proxyDataExists;

/**
 * The asset's renditions.
 * The renditions are keyed using the AdobePhotoAssetRenditionImage and AdobePhotoAssetRenditionVideo constants defined above.
 */
@property (nonatomic, readonly, strong) AdobePhotoAssetRenditionDictionary *renditions;

/**
 * The asset's size.
 */
@property (nonatomic, readonly, assign) long long size;

/**
 * The asset's type.
 */
@property (nonatomic, readonly, assign) AdobePhotoAssetType type;

/**
 * Asset creation async request.
 */
@property (nonatomic, readonly, strong) AdobePhotoAsyncRequest *createPhotoAsyncRequest;

/**
 * Create (upload) a new asset and add into a collection on the Adobe Photo service asynchronously.
 * For an image file: Preview, standard, and 2x thumbnail renditions are generated automatically.
 *
 * Upload can be cancelled via the createPhotoAsyncRequest property.
 *
 * @param name              The name of the asset.
 * @param collection        The collection to add the asset into.
 * @param path              The local path to the data. The path must be locally valid and cannot be nil.
 * @param type              The content-type of the asset. The content-type is required and cannot be nil.
 *                          Currently only kAdobeMimeTypeJPEG is supported.
 * @param progressBlock     Optionally track the upload progress.
 * @param successBlock      Optionally get an updated reference to the created AdobePhotoAsset if successful.
 * @param cancellationBlock Optionally be notified of a cancellation on upload.
 * @param errorBlock        Optionally be notified of an error.
 *
 * @return The newly created asset.
 *
 * @see AdobePhotoAsyncRequest
 */
+ (AdobePhotoAsset *)create:(NSString *)name
                 collection:(AdobePhotoCollection *)collection
                   dataPath:(NSURL *)path
                contentType:(NSString *)type
              progressBlock:(void (^)(double fractionCompleted))progressBlock
               successBlock:(void (^)(AdobePhotoAsset *asset))successBlock
          cancellationBlock:(void (^)(void))cancellationBlock
                 errorBlock:(void (^)(NSError *error))errorBlock;

/**
 * Create (upload) a new asset and add into a catalog on the Adobe Photo service asynchronously.
 * For an image file: Preview, standard, and 2x thumbnail renditions are generated automatically.
 *
 * Upload can be cancelled via the createPhotoAsyncRequest property.
 *
 * @param name              The name of the asset.
 * @param catalog           The catalog to add the asset into.
 * @param path              The local path to the data. The path must be locally valid and cannot be nil.
 * @param type              The content-type of the asset. The content-type is required and cannot be nil.
 *                          Currently only kAdobeMimeTypeJPEG is supported.
 * @param progressBlock     Optionally track the upload progress.
 * @param successBlock      Optionally get an updated reference to the created AdobePhotoAsset if successful.
 * @param cancellationBlock Optionally be notified of a cancellation on upload.
 * @param errorBlock        Optionally be notified of an error.
 *
 * @return The newly created asset.
 *
 * @see AdobePhotoAsyncRequest
 */
+ (AdobePhotoAsset *)create:(NSString *)name
                    catalog:(AdobePhotoCatalog *)catalog
                   dataPath:(NSURL *)path
                contentType:(NSString *)type
              progressBlock:(void (^)(double))progressBlock
               successBlock:(void (^)(AdobePhotoAsset *))successBlock
          cancellationBlock:(void (^)(void))cancellationBlock
                 errorBlock:(void (^)(NSError *))errorBlock;


/**
 * Refresh this asset from the Adobe Photo service asynchronously.
 *
 * @param successBlock  Optionally be notified if successful.
 * @param errorBlock    Optionally be notified of an error.
 */
- (void)refresh:(void (^)(AdobePhotoAsset *asset))successBlock
     errorBlock:(void (^)(NSError *error))errorBlock;

/**
 * Delete this asset from the Adobe Photo service asynchronously.
 *
 * @param successBlock  Optionally be notified if successful.
 * @param errorBlock    Optionally be notified of an error.
 */
- (void)delete:(void (^)(void))successBlock
    errorBlock:(void (^)(NSError *error))errorBlock;

/**
 * Upload the master data (limited to 200MB) for this asset on the Adobe Photo service asynchronously.]
 * The master data content type must be either: image/jpeg or application/octet-stream (for camera raw).
 * Since the connection may be unstable, we recommend that the data is no more that 20MB.
 *
 * @param path              The path to the master data. The path must be locally valid and cannot be nil.
 * @param priority          The priority of the request.
 * @param generate          If renditions should be automatically generated.
 * @param progressBlock     Optionally track the upload progress.
 * @param successBlock      Optionally get an updated reference to the asset if successful.
 * @param cancellationBlock Optionally be notified of a cancellation on upload.
 * @param errorBlock        Optionally be notified of an error.
 *
 * @return An @c AdobePhotoAsyncRequest which can be used to control the transfer priority and cancellation of the transfer.
 */
- (AdobePhotoAsyncRequest *)uploadMasterDataFromPath:(NSURL *)path
                                     requestPriority:(NSOperationQueuePriority)priority
                              autoGenerateRenditions:(BOOL)generate
                                       progressBlock:(void (^)(double fractionCompleted))progressBlock
                                        successBlock:(void (^)(AdobePhotoAsset *asset))successBlock
                                   cancellationBlock:(void (^)(void))cancellationBlock
                                          errorBlock:(void (^)(NSError *error))errorBlock;

/**
 * Upload a rendition for this asset on the Adobe Photo service asynchronously.
 *
 * @param rendition         The rendition object.
 * @param priority          The priority of the request.
 * @param progressBlock     Optionally track the upload progress.
 * @param successBlock      Optionally get an updated reference to the asset if successful.
 * @param cancellationBlock Optionally be notified of a cancellation on upload.
 * @param errorBlock        Optionally be notified of an error.
 *
 * @return An @c AdobePhotoAsyncRequest which can be used to control the transfer priority and cancellation of the transfer.
 */
- (AdobePhotoAsyncRequest *)uploadRendition:(AdobePhotoAssetRendition *)rendition
                            requestPriority:(NSOperationQueuePriority)priority
                              progressBlock:(void (^)(double fractionCompleted))progressBlock
                               successBlock:(void (^)(AdobePhotoAsset *asset))successBlock
                          cancellationBlock:(void (^)(void))cancellationBlock
                                 errorBlock:(void (^)(NSError *error))errorBlock;

/**
 * Get the master data for this asset on the Adobe Photo service asynchronously.
 *
 * Be careful with this API as the data size may be quite large (i.e. check size before pulling). To
 * download files of any size consider using
 * @c downloadMasterDataToFile:requestPriority:progressBlock:successBlock:cancellationBlock:errorBlock: which stores
 * the downloaded bits in a temporary file on disk.
 *
 * Note: a master file is not guaranteed to be associated with an asset. Check @c masterDataExists before making this call.
 *
 * @param priority          The priority of the request.
 * @param progressBlock     Optionally track the download progress.
 * @param successBlock      Returns the master data if successful.
 * @param cancellationBlock Optionally be notified of a cancellation on download.
 * @param errorBlock        Optionally be notified of an error.
 *
 * @return An @c AdobePhotoAsyncRequest which can be used to control the transfer priority and cancellation of the transfer.
 * @see downloadMasterDataToFile:requestPriority:progressBlock:successBlock:cancellationBlock:errorBlock:
 */
- (AdobePhotoAsyncRequest *)downloadMasterData:(NSOperationQueuePriority)priority
                                 progressBlock:(void (^)(double fractionCompleted))progressBlock
                                  successBlock:(void (^)(NSData *data, BOOL wasCached))successBlock
                             cancellationBlock:(void (^)(void))cancellationBlock
                                    errorBlock:(void (^)(NSError *error))errorBlock;

/**
 * Download the master data for the receiver from the Adobe Photo service asynchronously.
 *
 * The downloaded bits are stored in a temporary file on disk as they arrive. This prevents
 * excessive memory usage when downloading large files. Generally this method should be used and
 * preferred to @c downloadMasterData:progressBlock:successBlock:cancellationBlock:errorBlock:.
 *
 * Note: a master file is not guaranteed to be associated with an asset. Check @c masterDataExists before making this call.
 *
 * @param filePath            File system path where the final downloaded file should be stored.
 * @param priority            Priority of the request task.
 * @param progressBlock       Optionally track the download progress. Specify @c NULL to omit.
 * @param successBlock        Block to execute when the download task has completed successfully.
 * @param cancellationBlock   Optionally be notified of cancellation of the task. Specify @c NULL
 *                            to omit.
 * @param errorBlock          Optionally be notified of any errors. Specify @c NULL to omit.
 *
 * @return An @c AdobePhotoAsyncRequest which can be used to control the transfer priority and cancellation of the transfer.
 */
- (AdobePhotoAsyncRequest *)downloadMasterDataToFile:(NSString *)filePath
                                     requestPriority:(NSOperationQueuePriority)priority
                                       progressBlock:(void (^)(double fractionCompleted))progressBlock
                                        successBlock:(void (^)(AdobePhotoAsset *photoAsset))successBlock
                                   cancellationBlock:(void (^)(void))cancellationBlock
                                          errorBlock:(void (^)(NSError *error))errorBlock;

/**
 * Get the proxy data (DNG) for this asset on the Adobe Photo service asynchronously.
 *
 * Be careful with this API as the data size may be quite large (i.e. check size before pulling). To
 * download files of any size consider using
 * @c downloadProxyDataToFile:requestPriority:progressBlock:successBlock:cancellationBlock:errorBlock: which stores
 * the downloaded bits in a temporary file on disk.
 *
 * Note: a proxy file is not guaranteed to be associated with an asset. Check @c proxyDataExists before making this call.
 *
 * @param priority          The priority of the request.
 * @param progressBlock     Optionally track the download progress.
 * @param successBlock      Returns the master data if successful.
 * @param cancellationBlock Optionally be notified of a cancellation on download.
 * @param errorBlock        Optionally be notified of an error.
 *
 * @return An @c AdobePhotoAsyncRequest which can be used to control the transfer priority and cancellation of the transfer.
 */
- (AdobePhotoAsyncRequest *)downloadProxyData:(NSOperationQueuePriority)priority
                                progressBlock:(void (^)(double fractionCompleted))progressBlock
                                 successBlock:(void (^)(NSData *data, BOOL wasCached))successBlock
                            cancellationBlock:(void (^)(void))cancellationBlock
                                   errorBlock:(void (^)(NSError *error))errorBlock;

/**
 * Download the proxy data (DNG) for this asset from the Adobe Photo service asynchronously.
 *
 * The downloaded bits are stored in a temporary file on disk as they arrive. This prevents
 * excessive memory usage when downloading large files. Generally this method should be used and
 * preferred to @c downloadProxyData:progressBlock:successBlock:cancellationBlock:errorBlock:.
 *
 * Note: a proxy file is not guaranteed to be associated with an asset. Check @c proxyDataExists before making this call.
 *
 * @param filePath            File system path where the final downloaded file should be stored.
 * @param priority            Priority of the request task.
 * @param progressBlock       Optionally track the download progress. Specify @c NULL to omit.
 * @param successBlock        Block to execute when the download task has completed successfully.
 * @param cancellationBlock   Optionally be notified of cancellation of the task. Specify @c NULL to omit.
 * @param errorBlock          Optionally be notified of any errors. Specify @c NULL to omit.
 *
 * @return An @c AdobePhotoAsyncRequest which can be used to control the transfer priority and cancellation of the transfer.
 */
- (AdobePhotoAsyncRequest *)downloadProxyDataToFile:(NSString *)filePath
                                    requestPriority:(NSOperationQueuePriority)priority
                                      progressBlock:(void (^)(double fractionCompleted))progressBlock
                                       successBlock:(void (^)(AdobePhotoAsset *photoAsset))successBlock
                                  cancellationBlock:(void (^)(void))cancellationBlock
                                         errorBlock:(void (^)(NSError *error))errorBlock;

/**
 * Generate a series of renditions for this asset on the Adobe Photo service asynchronously.
 *
 * @param renditionTypes An array of AdobePhotoAssetRenditionType items to generate. To create a fullsize rendition that includes
 * metadata, pass only the single type AdobePhotoAssetRenditionTypeImageFullSize.
 * @param priority       The priority of the request.
 * @param successBlock   Returns the master data if successful.
 * @param errorBlock     Optionally be notified of an error.
 */
- (void)generateRenditions:(AdobePhotoAssetRenditionTypes *)renditionTypes
           requestPriority:(NSOperationQueuePriority)priority
              successBlock:(void (^)(AdobePhotoAsset *photoAsset))successBlock
                errorBlock:(void (^)(NSError *error))errorBlock;

/**
 * Get the rendition data for this asset on the Adobe Photo service asynchronously.
 * If the rendition is in the cache, it will return immediately with the data, else it will pull it from the service.
 *
 * @param rendition         The rendition to download.
 * @param priority          The priority of the request.
 * @param progressBlock     Optionally track the download progress.
 * @param successBlock      Returns the master data if successful.
 * @param cancellationBlock Optionally be notified of a cancellation on download.
 * @param errorBlock        Optionally be notified of an error.
 *
 * @return An @c AdobePhotoAsyncRequest which can be used to control the transfer priority and cancellation of the transfer.
 */
- (AdobePhotoAsyncRequest *)downloadRendition:(AdobePhotoAssetRendition *)rendition
                              requestPriority:(NSOperationQueuePriority)priority
                                progressBlock:(void (^)(double fractionCompleted))progressBlock
                                 successBlock:(void (^)(NSData *data, BOOL wasCached))successBlock
                            cancellationBlock:(void (^)(void))cancellationBlock
                                   errorBlock:(void (^)(NSError *error))errorBlock;

/**
 * Get the rendition data for this asset on the Adobe Photo service asynchronously.
 *
 * The downloaded bits are stored in a temporary file on disk as they arrive. This prevents
 * excessive memory usage when downloading larger files. The download will *not* be stored in our cache.
 *
 * @param filePath          File system path where the final downloaded file should be stored.
 * @param rendition         The rendition to download.
 * @param priority          The priority of the request.
 * @param progressBlock     Optionally track the download progress.
 * @param successBlock      Returns the master data if successful.
 * @param cancellationBlock Optionally be notified of a cancellation on download.
 * @param errorBlock        Optionally be notified of an error.
 *
 * @return An @c AdobePhotoAsyncRequest which can be used to control the transfer priority and cancellation of the transfer.
 */
- (AdobePhotoAsyncRequest *)downloadRendition:(AdobePhotoAssetRendition *)rendition
                                     filePath:(NSString *)filePath
                              requestPriority:(NSOperationQueuePriority)priority
                                progressBlock:(void (^)(double fractionCompleted))progressBlock
                                 successBlock:(void (^)(AdobePhotoAsset *photoAsset))successBlock
                            cancellationBlock:(void (^)(void))cancellationBlock
                                   errorBlock:(void (^)(NSError *error))errorBlock;

/**
 * A utility to test the equlity of two AdobePhotoAssets.
 *
 * @param asset The AdobePhotoAsset to test against.
 *
 * @return True if the assets are the same.
 */
- (BOOL)isEqualToAsset:(AdobePhotoAsset *)asset;

@end

#endif /* ifndef AdobePhotoAssetHeader */