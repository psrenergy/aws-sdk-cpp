﻿/**
 * Copyright Amazon.com, Inc. or its affiliates. All Rights Reserved.
 * SPDX-License-Identifier: Apache-2.0.
 */

#pragma once
#include <aws/omics/Omics_EXPORTS.h>
#include <aws/omics/OmicsRequest.h>
#include <aws/core/utils/memory/stl/AWSString.h>
#include <aws/omics/model/SseConfig.h>
#include <aws/core/utils/memory/stl/AWSMap.h>
#include <utility>

namespace Aws
{
namespace Omics
{
namespace Model
{

  /**
   */
  class AWS_OMICS_API CreateSequenceStoreRequest : public OmicsRequest
  {
  public:
    CreateSequenceStoreRequest();

    // Service request name is the Operation name which will send this request out,
    // each operation should has unique request name, so that we can get operation's name from this request.
    // Note: this is not true for response, multiple operations may have the same response name,
    // so we can not get operation's name from response.
    inline virtual const char* GetServiceRequestName() const override { return "CreateSequenceStore"; }

    Aws::String SerializePayload() const override;


    /**
     * <p>To ensure that requests don't run multiple times, specify a unique token for
     * each request.</p>
     */
    inline const Aws::String& GetClientToken() const{ return m_clientToken; }

    /**
     * <p>To ensure that requests don't run multiple times, specify a unique token for
     * each request.</p>
     */
    inline bool ClientTokenHasBeenSet() const { return m_clientTokenHasBeenSet; }

    /**
     * <p>To ensure that requests don't run multiple times, specify a unique token for
     * each request.</p>
     */
    inline void SetClientToken(const Aws::String& value) { m_clientTokenHasBeenSet = true; m_clientToken = value; }

    /**
     * <p>To ensure that requests don't run multiple times, specify a unique token for
     * each request.</p>
     */
    inline void SetClientToken(Aws::String&& value) { m_clientTokenHasBeenSet = true; m_clientToken = std::move(value); }

    /**
     * <p>To ensure that requests don't run multiple times, specify a unique token for
     * each request.</p>
     */
    inline void SetClientToken(const char* value) { m_clientTokenHasBeenSet = true; m_clientToken.assign(value); }

    /**
     * <p>To ensure that requests don't run multiple times, specify a unique token for
     * each request.</p>
     */
    inline CreateSequenceStoreRequest& WithClientToken(const Aws::String& value) { SetClientToken(value); return *this;}

    /**
     * <p>To ensure that requests don't run multiple times, specify a unique token for
     * each request.</p>
     */
    inline CreateSequenceStoreRequest& WithClientToken(Aws::String&& value) { SetClientToken(std::move(value)); return *this;}

    /**
     * <p>To ensure that requests don't run multiple times, specify a unique token for
     * each request.</p>
     */
    inline CreateSequenceStoreRequest& WithClientToken(const char* value) { SetClientToken(value); return *this;}


    /**
     * <p>A description for the store.</p>
     */
    inline const Aws::String& GetDescription() const{ return m_description; }

    /**
     * <p>A description for the store.</p>
     */
    inline bool DescriptionHasBeenSet() const { return m_descriptionHasBeenSet; }

    /**
     * <p>A description for the store.</p>
     */
    inline void SetDescription(const Aws::String& value) { m_descriptionHasBeenSet = true; m_description = value; }

    /**
     * <p>A description for the store.</p>
     */
    inline void SetDescription(Aws::String&& value) { m_descriptionHasBeenSet = true; m_description = std::move(value); }

    /**
     * <p>A description for the store.</p>
     */
    inline void SetDescription(const char* value) { m_descriptionHasBeenSet = true; m_description.assign(value); }

    /**
     * <p>A description for the store.</p>
     */
    inline CreateSequenceStoreRequest& WithDescription(const Aws::String& value) { SetDescription(value); return *this;}

    /**
     * <p>A description for the store.</p>
     */
    inline CreateSequenceStoreRequest& WithDescription(Aws::String&& value) { SetDescription(std::move(value)); return *this;}

    /**
     * <p>A description for the store.</p>
     */
    inline CreateSequenceStoreRequest& WithDescription(const char* value) { SetDescription(value); return *this;}


    /**
     * <p>A name for the store.</p>
     */
    inline const Aws::String& GetName() const{ return m_name; }

    /**
     * <p>A name for the store.</p>
     */
    inline bool NameHasBeenSet() const { return m_nameHasBeenSet; }

    /**
     * <p>A name for the store.</p>
     */
    inline void SetName(const Aws::String& value) { m_nameHasBeenSet = true; m_name = value; }

    /**
     * <p>A name for the store.</p>
     */
    inline void SetName(Aws::String&& value) { m_nameHasBeenSet = true; m_name = std::move(value); }

    /**
     * <p>A name for the store.</p>
     */
    inline void SetName(const char* value) { m_nameHasBeenSet = true; m_name.assign(value); }

    /**
     * <p>A name for the store.</p>
     */
    inline CreateSequenceStoreRequest& WithName(const Aws::String& value) { SetName(value); return *this;}

    /**
     * <p>A name for the store.</p>
     */
    inline CreateSequenceStoreRequest& WithName(Aws::String&& value) { SetName(std::move(value)); return *this;}

    /**
     * <p>A name for the store.</p>
     */
    inline CreateSequenceStoreRequest& WithName(const char* value) { SetName(value); return *this;}


    /**
     * <p>Server-side encryption (SSE) settings for the store.</p>
     */
    inline const SseConfig& GetSseConfig() const{ return m_sseConfig; }

    /**
     * <p>Server-side encryption (SSE) settings for the store.</p>
     */
    inline bool SseConfigHasBeenSet() const { return m_sseConfigHasBeenSet; }

    /**
     * <p>Server-side encryption (SSE) settings for the store.</p>
     */
    inline void SetSseConfig(const SseConfig& value) { m_sseConfigHasBeenSet = true; m_sseConfig = value; }

    /**
     * <p>Server-side encryption (SSE) settings for the store.</p>
     */
    inline void SetSseConfig(SseConfig&& value) { m_sseConfigHasBeenSet = true; m_sseConfig = std::move(value); }

    /**
     * <p>Server-side encryption (SSE) settings for the store.</p>
     */
    inline CreateSequenceStoreRequest& WithSseConfig(const SseConfig& value) { SetSseConfig(value); return *this;}

    /**
     * <p>Server-side encryption (SSE) settings for the store.</p>
     */
    inline CreateSequenceStoreRequest& WithSseConfig(SseConfig&& value) { SetSseConfig(std::move(value)); return *this;}


    /**
     * <p>Tags for the store.</p>
     */
    inline const Aws::Map<Aws::String, Aws::String>& GetTags() const{ return m_tags; }

    /**
     * <p>Tags for the store.</p>
     */
    inline bool TagsHasBeenSet() const { return m_tagsHasBeenSet; }

    /**
     * <p>Tags for the store.</p>
     */
    inline void SetTags(const Aws::Map<Aws::String, Aws::String>& value) { m_tagsHasBeenSet = true; m_tags = value; }

    /**
     * <p>Tags for the store.</p>
     */
    inline void SetTags(Aws::Map<Aws::String, Aws::String>&& value) { m_tagsHasBeenSet = true; m_tags = std::move(value); }

    /**
     * <p>Tags for the store.</p>
     */
    inline CreateSequenceStoreRequest& WithTags(const Aws::Map<Aws::String, Aws::String>& value) { SetTags(value); return *this;}

    /**
     * <p>Tags for the store.</p>
     */
    inline CreateSequenceStoreRequest& WithTags(Aws::Map<Aws::String, Aws::String>&& value) { SetTags(std::move(value)); return *this;}

    /**
     * <p>Tags for the store.</p>
     */
    inline CreateSequenceStoreRequest& AddTags(const Aws::String& key, const Aws::String& value) { m_tagsHasBeenSet = true; m_tags.emplace(key, value); return *this; }

    /**
     * <p>Tags for the store.</p>
     */
    inline CreateSequenceStoreRequest& AddTags(Aws::String&& key, const Aws::String& value) { m_tagsHasBeenSet = true; m_tags.emplace(std::move(key), value); return *this; }

    /**
     * <p>Tags for the store.</p>
     */
    inline CreateSequenceStoreRequest& AddTags(const Aws::String& key, Aws::String&& value) { m_tagsHasBeenSet = true; m_tags.emplace(key, std::move(value)); return *this; }

    /**
     * <p>Tags for the store.</p>
     */
    inline CreateSequenceStoreRequest& AddTags(Aws::String&& key, Aws::String&& value) { m_tagsHasBeenSet = true; m_tags.emplace(std::move(key), std::move(value)); return *this; }

    /**
     * <p>Tags for the store.</p>
     */
    inline CreateSequenceStoreRequest& AddTags(const char* key, Aws::String&& value) { m_tagsHasBeenSet = true; m_tags.emplace(key, std::move(value)); return *this; }

    /**
     * <p>Tags for the store.</p>
     */
    inline CreateSequenceStoreRequest& AddTags(Aws::String&& key, const char* value) { m_tagsHasBeenSet = true; m_tags.emplace(std::move(key), value); return *this; }

    /**
     * <p>Tags for the store.</p>
     */
    inline CreateSequenceStoreRequest& AddTags(const char* key, const char* value) { m_tagsHasBeenSet = true; m_tags.emplace(key, value); return *this; }

  private:

    Aws::String m_clientToken;
    bool m_clientTokenHasBeenSet = false;

    Aws::String m_description;
    bool m_descriptionHasBeenSet = false;

    Aws::String m_name;
    bool m_nameHasBeenSet = false;

    SseConfig m_sseConfig;
    bool m_sseConfigHasBeenSet = false;

    Aws::Map<Aws::String, Aws::String> m_tags;
    bool m_tagsHasBeenSet = false;
  };

} // namespace Model
} // namespace Omics
} // namespace Aws
