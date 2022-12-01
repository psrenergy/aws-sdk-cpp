﻿/**
 * Copyright Amazon.com, Inc. or its affiliates. All Rights Reserved.
 * SPDX-License-Identifier: Apache-2.0.
 */

#pragma once
#include <aws/chime-sdk-media-pipelines/ChimeSDKMediaPipelines_EXPORTS.h>
#include <aws/chime-sdk-media-pipelines/ChimeSDKMediaPipelinesRequest.h>
#include <aws/core/utils/memory/stl/AWSVector.h>
#include <aws/core/utils/memory/stl/AWSString.h>
#include <aws/chime-sdk-media-pipelines/model/LiveConnectorSourceConfiguration.h>
#include <aws/chime-sdk-media-pipelines/model/LiveConnectorSinkConfiguration.h>
#include <aws/chime-sdk-media-pipelines/model/Tag.h>
#include <utility>
#include <aws/core/utils/UUID.h>

namespace Aws
{
namespace ChimeSDKMediaPipelines
{
namespace Model
{

  /**
   */
  class AWS_CHIMESDKMEDIAPIPELINES_API CreateMediaLiveConnectorPipelineRequest : public ChimeSDKMediaPipelinesRequest
  {
  public:
    CreateMediaLiveConnectorPipelineRequest();

    /**
     * Virtual Copy Constructor idiom;
     * Override in an inherited class to have overridden functions (such as GetRequestSpecificHeaders) be called by the SDK.
     * (If you are creating a child of this: don't forget to keep the original return type or (better) just use the 'override' keyword)
    */
    virtual Aws::UniquePtr<CreateMediaLiveConnectorPipelineRequest> Clone() const
    {
      return Aws::MakeUnique<CreateMediaLiveConnectorPipelineRequest>(GetServiceRequestName(), *this);
    }

    // Service request name is the Operation name which will send this request out,
    // each operation should has unique request name, so that we can get operation's name from this request.
    // Note: this is not true for response, multiple operations may have the same response name,
    // so we can not get operation's name from response.
    inline virtual const char* GetServiceRequestName() const override { return "CreateMediaLiveConnectorPipeline"; }

    Aws::String SerializePayload() const override;


    /**
     * <p>The media pipeline's data sources.</p>
     */
    inline const Aws::Vector<LiveConnectorSourceConfiguration>& GetSources() const{ return m_sources; }

    /**
     * <p>The media pipeline's data sources.</p>
     */
    inline bool SourcesHasBeenSet() const { return m_sourcesHasBeenSet; }

    /**
     * <p>The media pipeline's data sources.</p>
     */
    inline void SetSources(const Aws::Vector<LiveConnectorSourceConfiguration>& value) { m_sourcesHasBeenSet = true; m_sources = value; }

    /**
     * <p>The media pipeline's data sources.</p>
     */
    inline void SetSources(Aws::Vector<LiveConnectorSourceConfiguration>&& value) { m_sourcesHasBeenSet = true; m_sources = std::move(value); }

    /**
     * <p>The media pipeline's data sources.</p>
     */
    inline CreateMediaLiveConnectorPipelineRequest& WithSources(const Aws::Vector<LiveConnectorSourceConfiguration>& value) { SetSources(value); return *this;}

    /**
     * <p>The media pipeline's data sources.</p>
     */
    inline CreateMediaLiveConnectorPipelineRequest& WithSources(Aws::Vector<LiveConnectorSourceConfiguration>&& value) { SetSources(std::move(value)); return *this;}

    /**
     * <p>The media pipeline's data sources.</p>
     */
    inline CreateMediaLiveConnectorPipelineRequest& AddSources(const LiveConnectorSourceConfiguration& value) { m_sourcesHasBeenSet = true; m_sources.push_back(value); return *this; }

    /**
     * <p>The media pipeline's data sources.</p>
     */
    inline CreateMediaLiveConnectorPipelineRequest& AddSources(LiveConnectorSourceConfiguration&& value) { m_sourcesHasBeenSet = true; m_sources.push_back(std::move(value)); return *this; }


    /**
     * <p>The media pipeline's data sinks.</p>
     */
    inline const Aws::Vector<LiveConnectorSinkConfiguration>& GetSinks() const{ return m_sinks; }

    /**
     * <p>The media pipeline's data sinks.</p>
     */
    inline bool SinksHasBeenSet() const { return m_sinksHasBeenSet; }

    /**
     * <p>The media pipeline's data sinks.</p>
     */
    inline void SetSinks(const Aws::Vector<LiveConnectorSinkConfiguration>& value) { m_sinksHasBeenSet = true; m_sinks = value; }

    /**
     * <p>The media pipeline's data sinks.</p>
     */
    inline void SetSinks(Aws::Vector<LiveConnectorSinkConfiguration>&& value) { m_sinksHasBeenSet = true; m_sinks = std::move(value); }

    /**
     * <p>The media pipeline's data sinks.</p>
     */
    inline CreateMediaLiveConnectorPipelineRequest& WithSinks(const Aws::Vector<LiveConnectorSinkConfiguration>& value) { SetSinks(value); return *this;}

    /**
     * <p>The media pipeline's data sinks.</p>
     */
    inline CreateMediaLiveConnectorPipelineRequest& WithSinks(Aws::Vector<LiveConnectorSinkConfiguration>&& value) { SetSinks(std::move(value)); return *this;}

    /**
     * <p>The media pipeline's data sinks.</p>
     */
    inline CreateMediaLiveConnectorPipelineRequest& AddSinks(const LiveConnectorSinkConfiguration& value) { m_sinksHasBeenSet = true; m_sinks.push_back(value); return *this; }

    /**
     * <p>The media pipeline's data sinks.</p>
     */
    inline CreateMediaLiveConnectorPipelineRequest& AddSinks(LiveConnectorSinkConfiguration&& value) { m_sinksHasBeenSet = true; m_sinks.push_back(std::move(value)); return *this; }


    /**
     * <p>The token assigned to the client making the request.</p>
     */
    inline const Aws::String& GetClientRequestToken() const{ return m_clientRequestToken; }

    /**
     * <p>The token assigned to the client making the request.</p>
     */
    inline bool ClientRequestTokenHasBeenSet() const { return m_clientRequestTokenHasBeenSet; }

    /**
     * <p>The token assigned to the client making the request.</p>
     */
    inline void SetClientRequestToken(const Aws::String& value) { m_clientRequestTokenHasBeenSet = true; m_clientRequestToken = value; }

    /**
     * <p>The token assigned to the client making the request.</p>
     */
    inline void SetClientRequestToken(Aws::String&& value) { m_clientRequestTokenHasBeenSet = true; m_clientRequestToken = std::move(value); }

    /**
     * <p>The token assigned to the client making the request.</p>
     */
    inline void SetClientRequestToken(const char* value) { m_clientRequestTokenHasBeenSet = true; m_clientRequestToken.assign(value); }

    /**
     * <p>The token assigned to the client making the request.</p>
     */
    inline CreateMediaLiveConnectorPipelineRequest& WithClientRequestToken(const Aws::String& value) { SetClientRequestToken(value); return *this;}

    /**
     * <p>The token assigned to the client making the request.</p>
     */
    inline CreateMediaLiveConnectorPipelineRequest& WithClientRequestToken(Aws::String&& value) { SetClientRequestToken(std::move(value)); return *this;}

    /**
     * <p>The token assigned to the client making the request.</p>
     */
    inline CreateMediaLiveConnectorPipelineRequest& WithClientRequestToken(const char* value) { SetClientRequestToken(value); return *this;}


    /**
     * <p>The tags associated with the media pipeline.</p>
     */
    inline const Aws::Vector<Tag>& GetTags() const{ return m_tags; }

    /**
     * <p>The tags associated with the media pipeline.</p>
     */
    inline bool TagsHasBeenSet() const { return m_tagsHasBeenSet; }

    /**
     * <p>The tags associated with the media pipeline.</p>
     */
    inline void SetTags(const Aws::Vector<Tag>& value) { m_tagsHasBeenSet = true; m_tags = value; }

    /**
     * <p>The tags associated with the media pipeline.</p>
     */
    inline void SetTags(Aws::Vector<Tag>&& value) { m_tagsHasBeenSet = true; m_tags = std::move(value); }

    /**
     * <p>The tags associated with the media pipeline.</p>
     */
    inline CreateMediaLiveConnectorPipelineRequest& WithTags(const Aws::Vector<Tag>& value) { SetTags(value); return *this;}

    /**
     * <p>The tags associated with the media pipeline.</p>
     */
    inline CreateMediaLiveConnectorPipelineRequest& WithTags(Aws::Vector<Tag>&& value) { SetTags(std::move(value)); return *this;}

    /**
     * <p>The tags associated with the media pipeline.</p>
     */
    inline CreateMediaLiveConnectorPipelineRequest& AddTags(const Tag& value) { m_tagsHasBeenSet = true; m_tags.push_back(value); return *this; }

    /**
     * <p>The tags associated with the media pipeline.</p>
     */
    inline CreateMediaLiveConnectorPipelineRequest& AddTags(Tag&& value) { m_tagsHasBeenSet = true; m_tags.push_back(std::move(value)); return *this; }

  private:

    Aws::Vector<LiveConnectorSourceConfiguration> m_sources;
    bool m_sourcesHasBeenSet = false;

    Aws::Vector<LiveConnectorSinkConfiguration> m_sinks;
    bool m_sinksHasBeenSet = false;

    Aws::String m_clientRequestToken;
    bool m_clientRequestTokenHasBeenSet = false;

    Aws::Vector<Tag> m_tags;
    bool m_tagsHasBeenSet = false;
  };

} // namespace Model
} // namespace ChimeSDKMediaPipelines
} // namespace Aws
