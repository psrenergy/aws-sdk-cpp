﻿/**
 * Copyright Amazon.com, Inc. or its affiliates. All Rights Reserved.
 * SPDX-License-Identifier: Apache-2.0.
 */

#pragma once
#include <aws/mediapackage/MediaPackage_EXPORTS.h>
#include <aws/mediapackage/model/CmafEncryption.h>
#include <aws/core/utils/memory/stl/AWSVector.h>
#include <aws/core/utils/memory/stl/AWSString.h>
#include <aws/mediapackage/model/StreamSelection.h>
#include <aws/mediapackage/model/HlsManifest.h>
#include <utility>

namespace Aws
{
namespace Utils
{
namespace Json
{
  class JsonValue;
  class JsonView;
} // namespace Json
} // namespace Utils
namespace MediaPackage
{
namespace Model
{

  /**
   * A Common Media Application Format (CMAF) packaging configuration.<p><h3>See
   * Also:</h3>   <a
   * href="http://docs.aws.amazon.com/goto/WebAPI/mediapackage-2017-10-12/CmafPackage">AWS
   * API Reference</a></p>
   */
  class AWS_MEDIAPACKAGE_API CmafPackage
  {
  public:
    CmafPackage();
    CmafPackage(Aws::Utils::Json::JsonView jsonValue);
    CmafPackage& operator=(Aws::Utils::Json::JsonView jsonValue);
    Aws::Utils::Json::JsonValue Jsonize() const;


    
    inline const CmafEncryption& GetEncryption() const{ return m_encryption; }

    
    inline bool EncryptionHasBeenSet() const { return m_encryptionHasBeenSet; }

    
    inline void SetEncryption(const CmafEncryption& value) { m_encryptionHasBeenSet = true; m_encryption = value; }

    
    inline void SetEncryption(CmafEncryption&& value) { m_encryptionHasBeenSet = true; m_encryption = std::move(value); }

    
    inline CmafPackage& WithEncryption(const CmafEncryption& value) { SetEncryption(value); return *this;}

    
    inline CmafPackage& WithEncryption(CmafEncryption&& value) { SetEncryption(std::move(value)); return *this;}


    /**
     * A list of HLS manifest configurations
     */
    inline const Aws::Vector<HlsManifest>& GetHlsManifests() const{ return m_hlsManifests; }

    /**
     * A list of HLS manifest configurations
     */
    inline bool HlsManifestsHasBeenSet() const { return m_hlsManifestsHasBeenSet; }

    /**
     * A list of HLS manifest configurations
     */
    inline void SetHlsManifests(const Aws::Vector<HlsManifest>& value) { m_hlsManifestsHasBeenSet = true; m_hlsManifests = value; }

    /**
     * A list of HLS manifest configurations
     */
    inline void SetHlsManifests(Aws::Vector<HlsManifest>&& value) { m_hlsManifestsHasBeenSet = true; m_hlsManifests = std::move(value); }

    /**
     * A list of HLS manifest configurations
     */
    inline CmafPackage& WithHlsManifests(const Aws::Vector<HlsManifest>& value) { SetHlsManifests(value); return *this;}

    /**
     * A list of HLS manifest configurations
     */
    inline CmafPackage& WithHlsManifests(Aws::Vector<HlsManifest>&& value) { SetHlsManifests(std::move(value)); return *this;}

    /**
     * A list of HLS manifest configurations
     */
    inline CmafPackage& AddHlsManifests(const HlsManifest& value) { m_hlsManifestsHasBeenSet = true; m_hlsManifests.push_back(value); return *this; }

    /**
     * A list of HLS manifest configurations
     */
    inline CmafPackage& AddHlsManifests(HlsManifest&& value) { m_hlsManifestsHasBeenSet = true; m_hlsManifests.push_back(std::move(value)); return *this; }


    /**
     * Duration (in seconds) of each segment. Actual segments will be
rounded to the
     * nearest multiple of the source segment duration.

     */
    inline int GetSegmentDurationSeconds() const{ return m_segmentDurationSeconds; }

    /**
     * Duration (in seconds) of each segment. Actual segments will be
rounded to the
     * nearest multiple of the source segment duration.

     */
    inline bool SegmentDurationSecondsHasBeenSet() const { return m_segmentDurationSecondsHasBeenSet; }

    /**
     * Duration (in seconds) of each segment. Actual segments will be
rounded to the
     * nearest multiple of the source segment duration.

     */
    inline void SetSegmentDurationSeconds(int value) { m_segmentDurationSecondsHasBeenSet = true; m_segmentDurationSeconds = value; }

    /**
     * Duration (in seconds) of each segment. Actual segments will be
rounded to the
     * nearest multiple of the source segment duration.

     */
    inline CmafPackage& WithSegmentDurationSeconds(int value) { SetSegmentDurationSeconds(value); return *this;}


    /**
     * An optional custom string that is prepended to the name of each segment. If not
     * specified, it defaults to the ChannelId.
     */
    inline const Aws::String& GetSegmentPrefix() const{ return m_segmentPrefix; }

    /**
     * An optional custom string that is prepended to the name of each segment. If not
     * specified, it defaults to the ChannelId.
     */
    inline bool SegmentPrefixHasBeenSet() const { return m_segmentPrefixHasBeenSet; }

    /**
     * An optional custom string that is prepended to the name of each segment. If not
     * specified, it defaults to the ChannelId.
     */
    inline void SetSegmentPrefix(const Aws::String& value) { m_segmentPrefixHasBeenSet = true; m_segmentPrefix = value; }

    /**
     * An optional custom string that is prepended to the name of each segment. If not
     * specified, it defaults to the ChannelId.
     */
    inline void SetSegmentPrefix(Aws::String&& value) { m_segmentPrefixHasBeenSet = true; m_segmentPrefix = std::move(value); }

    /**
     * An optional custom string that is prepended to the name of each segment. If not
     * specified, it defaults to the ChannelId.
     */
    inline void SetSegmentPrefix(const char* value) { m_segmentPrefixHasBeenSet = true; m_segmentPrefix.assign(value); }

    /**
     * An optional custom string that is prepended to the name of each segment. If not
     * specified, it defaults to the ChannelId.
     */
    inline CmafPackage& WithSegmentPrefix(const Aws::String& value) { SetSegmentPrefix(value); return *this;}

    /**
     * An optional custom string that is prepended to the name of each segment. If not
     * specified, it defaults to the ChannelId.
     */
    inline CmafPackage& WithSegmentPrefix(Aws::String&& value) { SetSegmentPrefix(std::move(value)); return *this;}

    /**
     * An optional custom string that is prepended to the name of each segment. If not
     * specified, it defaults to the ChannelId.
     */
    inline CmafPackage& WithSegmentPrefix(const char* value) { SetSegmentPrefix(value); return *this;}


    
    inline const StreamSelection& GetStreamSelection() const{ return m_streamSelection; }

    
    inline bool StreamSelectionHasBeenSet() const { return m_streamSelectionHasBeenSet; }

    
    inline void SetStreamSelection(const StreamSelection& value) { m_streamSelectionHasBeenSet = true; m_streamSelection = value; }

    
    inline void SetStreamSelection(StreamSelection&& value) { m_streamSelectionHasBeenSet = true; m_streamSelection = std::move(value); }

    
    inline CmafPackage& WithStreamSelection(const StreamSelection& value) { SetStreamSelection(value); return *this;}

    
    inline CmafPackage& WithStreamSelection(StreamSelection&& value) { SetStreamSelection(std::move(value)); return *this;}

  private:

    CmafEncryption m_encryption;
    bool m_encryptionHasBeenSet = false;

    Aws::Vector<HlsManifest> m_hlsManifests;
    bool m_hlsManifestsHasBeenSet = false;

    int m_segmentDurationSeconds;
    bool m_segmentDurationSecondsHasBeenSet = false;

    Aws::String m_segmentPrefix;
    bool m_segmentPrefixHasBeenSet = false;

    StreamSelection m_streamSelection;
    bool m_streamSelectionHasBeenSet = false;
  };

} // namespace Model
} // namespace MediaPackage
} // namespace Aws
