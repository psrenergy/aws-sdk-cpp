﻿/**
 * Copyright Amazon.com, Inc. or its affiliates. All Rights Reserved.
 * SPDX-License-Identifier: Apache-2.0.
 */

#pragma once
#include <aws/oam/OAM_EXPORTS.h>
#include <aws/core/utils/memory/stl/AWSString.h>
#include <aws/core/utils/memory/stl/AWSVector.h>
#include <aws/core/utils/memory/stl/AWSMap.h>
#include <utility>

namespace Aws
{
template<typename RESULT_TYPE>
class AmazonWebServiceResult;

namespace Utils
{
namespace Json
{
  class JsonValue;
} // namespace Json
} // namespace Utils
namespace OAM
{
namespace Model
{
  class AWS_OAM_API GetLinkResult
  {
  public:
    GetLinkResult();
    GetLinkResult(const Aws::AmazonWebServiceResult<Aws::Utils::Json::JsonValue>& result);
    GetLinkResult& operator=(const Aws::AmazonWebServiceResult<Aws::Utils::Json::JsonValue>& result);


    /**
     * <p>The ARN of the link.</p>
     */
    inline const Aws::String& GetArn() const{ return m_arn; }

    /**
     * <p>The ARN of the link.</p>
     */
    inline void SetArn(const Aws::String& value) { m_arn = value; }

    /**
     * <p>The ARN of the link.</p>
     */
    inline void SetArn(Aws::String&& value) { m_arn = std::move(value); }

    /**
     * <p>The ARN of the link.</p>
     */
    inline void SetArn(const char* value) { m_arn.assign(value); }

    /**
     * <p>The ARN of the link.</p>
     */
    inline GetLinkResult& WithArn(const Aws::String& value) { SetArn(value); return *this;}

    /**
     * <p>The ARN of the link.</p>
     */
    inline GetLinkResult& WithArn(Aws::String&& value) { SetArn(std::move(value)); return *this;}

    /**
     * <p>The ARN of the link.</p>
     */
    inline GetLinkResult& WithArn(const char* value) { SetArn(value); return *this;}


    /**
     * <p>The random ID string that Amazon Web Services generated as part of the link
     * ARN.</p>
     */
    inline const Aws::String& GetId() const{ return m_id; }

    /**
     * <p>The random ID string that Amazon Web Services generated as part of the link
     * ARN.</p>
     */
    inline void SetId(const Aws::String& value) { m_id = value; }

    /**
     * <p>The random ID string that Amazon Web Services generated as part of the link
     * ARN.</p>
     */
    inline void SetId(Aws::String&& value) { m_id = std::move(value); }

    /**
     * <p>The random ID string that Amazon Web Services generated as part of the link
     * ARN.</p>
     */
    inline void SetId(const char* value) { m_id.assign(value); }

    /**
     * <p>The random ID string that Amazon Web Services generated as part of the link
     * ARN.</p>
     */
    inline GetLinkResult& WithId(const Aws::String& value) { SetId(value); return *this;}

    /**
     * <p>The random ID string that Amazon Web Services generated as part of the link
     * ARN.</p>
     */
    inline GetLinkResult& WithId(Aws::String&& value) { SetId(std::move(value)); return *this;}

    /**
     * <p>The random ID string that Amazon Web Services generated as part of the link
     * ARN.</p>
     */
    inline GetLinkResult& WithId(const char* value) { SetId(value); return *this;}


    /**
     * <p>The label that you assigned to this link, with the variables resolved to
     * their actual values.</p>
     */
    inline const Aws::String& GetLabel() const{ return m_label; }

    /**
     * <p>The label that you assigned to this link, with the variables resolved to
     * their actual values.</p>
     */
    inline void SetLabel(const Aws::String& value) { m_label = value; }

    /**
     * <p>The label that you assigned to this link, with the variables resolved to
     * their actual values.</p>
     */
    inline void SetLabel(Aws::String&& value) { m_label = std::move(value); }

    /**
     * <p>The label that you assigned to this link, with the variables resolved to
     * their actual values.</p>
     */
    inline void SetLabel(const char* value) { m_label.assign(value); }

    /**
     * <p>The label that you assigned to this link, with the variables resolved to
     * their actual values.</p>
     */
    inline GetLinkResult& WithLabel(const Aws::String& value) { SetLabel(value); return *this;}

    /**
     * <p>The label that you assigned to this link, with the variables resolved to
     * their actual values.</p>
     */
    inline GetLinkResult& WithLabel(Aws::String&& value) { SetLabel(std::move(value)); return *this;}

    /**
     * <p>The label that you assigned to this link, with the variables resolved to
     * their actual values.</p>
     */
    inline GetLinkResult& WithLabel(const char* value) { SetLabel(value); return *this;}


    /**
     * <p>The exact label template that was specified when the link was created, with
     * the template variables not resolved.</p>
     */
    inline const Aws::String& GetLabelTemplate() const{ return m_labelTemplate; }

    /**
     * <p>The exact label template that was specified when the link was created, with
     * the template variables not resolved.</p>
     */
    inline void SetLabelTemplate(const Aws::String& value) { m_labelTemplate = value; }

    /**
     * <p>The exact label template that was specified when the link was created, with
     * the template variables not resolved.</p>
     */
    inline void SetLabelTemplate(Aws::String&& value) { m_labelTemplate = std::move(value); }

    /**
     * <p>The exact label template that was specified when the link was created, with
     * the template variables not resolved.</p>
     */
    inline void SetLabelTemplate(const char* value) { m_labelTemplate.assign(value); }

    /**
     * <p>The exact label template that was specified when the link was created, with
     * the template variables not resolved.</p>
     */
    inline GetLinkResult& WithLabelTemplate(const Aws::String& value) { SetLabelTemplate(value); return *this;}

    /**
     * <p>The exact label template that was specified when the link was created, with
     * the template variables not resolved.</p>
     */
    inline GetLinkResult& WithLabelTemplate(Aws::String&& value) { SetLabelTemplate(std::move(value)); return *this;}

    /**
     * <p>The exact label template that was specified when the link was created, with
     * the template variables not resolved.</p>
     */
    inline GetLinkResult& WithLabelTemplate(const char* value) { SetLabelTemplate(value); return *this;}


    /**
     * <p>The resource types supported by this link.</p>
     */
    inline const Aws::Vector<Aws::String>& GetResourceTypes() const{ return m_resourceTypes; }

    /**
     * <p>The resource types supported by this link.</p>
     */
    inline void SetResourceTypes(const Aws::Vector<Aws::String>& value) { m_resourceTypes = value; }

    /**
     * <p>The resource types supported by this link.</p>
     */
    inline void SetResourceTypes(Aws::Vector<Aws::String>&& value) { m_resourceTypes = std::move(value); }

    /**
     * <p>The resource types supported by this link.</p>
     */
    inline GetLinkResult& WithResourceTypes(const Aws::Vector<Aws::String>& value) { SetResourceTypes(value); return *this;}

    /**
     * <p>The resource types supported by this link.</p>
     */
    inline GetLinkResult& WithResourceTypes(Aws::Vector<Aws::String>&& value) { SetResourceTypes(std::move(value)); return *this;}

    /**
     * <p>The resource types supported by this link.</p>
     */
    inline GetLinkResult& AddResourceTypes(const Aws::String& value) { m_resourceTypes.push_back(value); return *this; }

    /**
     * <p>The resource types supported by this link.</p>
     */
    inline GetLinkResult& AddResourceTypes(Aws::String&& value) { m_resourceTypes.push_back(std::move(value)); return *this; }

    /**
     * <p>The resource types supported by this link.</p>
     */
    inline GetLinkResult& AddResourceTypes(const char* value) { m_resourceTypes.push_back(value); return *this; }


    /**
     * <p>The ARN of the sink that is used for this link.</p>
     */
    inline const Aws::String& GetSinkArn() const{ return m_sinkArn; }

    /**
     * <p>The ARN of the sink that is used for this link.</p>
     */
    inline void SetSinkArn(const Aws::String& value) { m_sinkArn = value; }

    /**
     * <p>The ARN of the sink that is used for this link.</p>
     */
    inline void SetSinkArn(Aws::String&& value) { m_sinkArn = std::move(value); }

    /**
     * <p>The ARN of the sink that is used for this link.</p>
     */
    inline void SetSinkArn(const char* value) { m_sinkArn.assign(value); }

    /**
     * <p>The ARN of the sink that is used for this link.</p>
     */
    inline GetLinkResult& WithSinkArn(const Aws::String& value) { SetSinkArn(value); return *this;}

    /**
     * <p>The ARN of the sink that is used for this link.</p>
     */
    inline GetLinkResult& WithSinkArn(Aws::String&& value) { SetSinkArn(std::move(value)); return *this;}

    /**
     * <p>The ARN of the sink that is used for this link.</p>
     */
    inline GetLinkResult& WithSinkArn(const char* value) { SetSinkArn(value); return *this;}


    /**
     * <p>The tags assigned to the link.</p>
     */
    inline const Aws::Map<Aws::String, Aws::String>& GetTags() const{ return m_tags; }

    /**
     * <p>The tags assigned to the link.</p>
     */
    inline void SetTags(const Aws::Map<Aws::String, Aws::String>& value) { m_tags = value; }

    /**
     * <p>The tags assigned to the link.</p>
     */
    inline void SetTags(Aws::Map<Aws::String, Aws::String>&& value) { m_tags = std::move(value); }

    /**
     * <p>The tags assigned to the link.</p>
     */
    inline GetLinkResult& WithTags(const Aws::Map<Aws::String, Aws::String>& value) { SetTags(value); return *this;}

    /**
     * <p>The tags assigned to the link.</p>
     */
    inline GetLinkResult& WithTags(Aws::Map<Aws::String, Aws::String>&& value) { SetTags(std::move(value)); return *this;}

    /**
     * <p>The tags assigned to the link.</p>
     */
    inline GetLinkResult& AddTags(const Aws::String& key, const Aws::String& value) { m_tags.emplace(key, value); return *this; }

    /**
     * <p>The tags assigned to the link.</p>
     */
    inline GetLinkResult& AddTags(Aws::String&& key, const Aws::String& value) { m_tags.emplace(std::move(key), value); return *this; }

    /**
     * <p>The tags assigned to the link.</p>
     */
    inline GetLinkResult& AddTags(const Aws::String& key, Aws::String&& value) { m_tags.emplace(key, std::move(value)); return *this; }

    /**
     * <p>The tags assigned to the link.</p>
     */
    inline GetLinkResult& AddTags(Aws::String&& key, Aws::String&& value) { m_tags.emplace(std::move(key), std::move(value)); return *this; }

    /**
     * <p>The tags assigned to the link.</p>
     */
    inline GetLinkResult& AddTags(const char* key, Aws::String&& value) { m_tags.emplace(key, std::move(value)); return *this; }

    /**
     * <p>The tags assigned to the link.</p>
     */
    inline GetLinkResult& AddTags(Aws::String&& key, const char* value) { m_tags.emplace(std::move(key), value); return *this; }

    /**
     * <p>The tags assigned to the link.</p>
     */
    inline GetLinkResult& AddTags(const char* key, const char* value) { m_tags.emplace(key, value); return *this; }

  private:

    Aws::String m_arn;

    Aws::String m_id;

    Aws::String m_label;

    Aws::String m_labelTemplate;

    Aws::Vector<Aws::String> m_resourceTypes;

    Aws::String m_sinkArn;

    Aws::Map<Aws::String, Aws::String> m_tags;
  };

} // namespace Model
} // namespace OAM
} // namespace Aws
