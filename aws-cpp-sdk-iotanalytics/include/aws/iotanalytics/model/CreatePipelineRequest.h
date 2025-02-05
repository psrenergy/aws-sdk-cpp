﻿/**
 * Copyright Amazon.com, Inc. or its affiliates. All Rights Reserved.
 * SPDX-License-Identifier: Apache-2.0.
 */

#pragma once
#include <aws/iotanalytics/IoTAnalytics_EXPORTS.h>
#include <aws/iotanalytics/IoTAnalyticsRequest.h>
#include <aws/core/utils/memory/stl/AWSString.h>
#include <aws/core/utils/memory/stl/AWSVector.h>
#include <aws/iotanalytics/model/PipelineActivity.h>
#include <aws/iotanalytics/model/Tag.h>
#include <utility>

namespace Aws
{
namespace IoTAnalytics
{
namespace Model
{

  /**
   */
  class AWS_IOTANALYTICS_API CreatePipelineRequest : public IoTAnalyticsRequest
  {
  public:
    CreatePipelineRequest();

    // Service request name is the Operation name which will send this request out,
    // each operation should has unique request name, so that we can get operation's name from this request.
    // Note: this is not true for response, multiple operations may have the same response name,
    // so we can not get operation's name from response.
    inline virtual const char* GetServiceRequestName() const override { return "CreatePipeline"; }

    Aws::String SerializePayload() const override;


    /**
     * <p>The name of the pipeline.</p>
     */
    inline const Aws::String& GetPipelineName() const{ return m_pipelineName; }

    /**
     * <p>The name of the pipeline.</p>
     */
    inline bool PipelineNameHasBeenSet() const { return m_pipelineNameHasBeenSet; }

    /**
     * <p>The name of the pipeline.</p>
     */
    inline void SetPipelineName(const Aws::String& value) { m_pipelineNameHasBeenSet = true; m_pipelineName = value; }

    /**
     * <p>The name of the pipeline.</p>
     */
    inline void SetPipelineName(Aws::String&& value) { m_pipelineNameHasBeenSet = true; m_pipelineName = std::move(value); }

    /**
     * <p>The name of the pipeline.</p>
     */
    inline void SetPipelineName(const char* value) { m_pipelineNameHasBeenSet = true; m_pipelineName.assign(value); }

    /**
     * <p>The name of the pipeline.</p>
     */
    inline CreatePipelineRequest& WithPipelineName(const Aws::String& value) { SetPipelineName(value); return *this;}

    /**
     * <p>The name of the pipeline.</p>
     */
    inline CreatePipelineRequest& WithPipelineName(Aws::String&& value) { SetPipelineName(std::move(value)); return *this;}

    /**
     * <p>The name of the pipeline.</p>
     */
    inline CreatePipelineRequest& WithPipelineName(const char* value) { SetPipelineName(value); return *this;}


    /**
     * <p>A list of <code>PipelineActivity</code> objects. Activities perform
     * transformations on your messages, such as removing, renaming or adding message
     * attributes; filtering messages based on attribute values; invoking your Lambda
     * unctions on messages for advanced processing; or performing mathematical
     * transformations to normalize device data.</p> <p>The list can be 2-25
     * <code>PipelineActivity</code> objects and must contain both a
     * <code>channel</code> and a <code>datastore</code> activity. Each entry in the
     * list must contain only one activity. For example:</p> <p>
     * <code>pipelineActivities = [ { "channel": { ... } }, { "lambda": { ... } }, ...
     * ]</code> </p>
     */
    inline const Aws::Vector<PipelineActivity>& GetPipelineActivities() const{ return m_pipelineActivities; }

    /**
     * <p>A list of <code>PipelineActivity</code> objects. Activities perform
     * transformations on your messages, such as removing, renaming or adding message
     * attributes; filtering messages based on attribute values; invoking your Lambda
     * unctions on messages for advanced processing; or performing mathematical
     * transformations to normalize device data.</p> <p>The list can be 2-25
     * <code>PipelineActivity</code> objects and must contain both a
     * <code>channel</code> and a <code>datastore</code> activity. Each entry in the
     * list must contain only one activity. For example:</p> <p>
     * <code>pipelineActivities = [ { "channel": { ... } }, { "lambda": { ... } }, ...
     * ]</code> </p>
     */
    inline bool PipelineActivitiesHasBeenSet() const { return m_pipelineActivitiesHasBeenSet; }

    /**
     * <p>A list of <code>PipelineActivity</code> objects. Activities perform
     * transformations on your messages, such as removing, renaming or adding message
     * attributes; filtering messages based on attribute values; invoking your Lambda
     * unctions on messages for advanced processing; or performing mathematical
     * transformations to normalize device data.</p> <p>The list can be 2-25
     * <code>PipelineActivity</code> objects and must contain both a
     * <code>channel</code> and a <code>datastore</code> activity. Each entry in the
     * list must contain only one activity. For example:</p> <p>
     * <code>pipelineActivities = [ { "channel": { ... } }, { "lambda": { ... } }, ...
     * ]</code> </p>
     */
    inline void SetPipelineActivities(const Aws::Vector<PipelineActivity>& value) { m_pipelineActivitiesHasBeenSet = true; m_pipelineActivities = value; }

    /**
     * <p>A list of <code>PipelineActivity</code> objects. Activities perform
     * transformations on your messages, such as removing, renaming or adding message
     * attributes; filtering messages based on attribute values; invoking your Lambda
     * unctions on messages for advanced processing; or performing mathematical
     * transformations to normalize device data.</p> <p>The list can be 2-25
     * <code>PipelineActivity</code> objects and must contain both a
     * <code>channel</code> and a <code>datastore</code> activity. Each entry in the
     * list must contain only one activity. For example:</p> <p>
     * <code>pipelineActivities = [ { "channel": { ... } }, { "lambda": { ... } }, ...
     * ]</code> </p>
     */
    inline void SetPipelineActivities(Aws::Vector<PipelineActivity>&& value) { m_pipelineActivitiesHasBeenSet = true; m_pipelineActivities = std::move(value); }

    /**
     * <p>A list of <code>PipelineActivity</code> objects. Activities perform
     * transformations on your messages, such as removing, renaming or adding message
     * attributes; filtering messages based on attribute values; invoking your Lambda
     * unctions on messages for advanced processing; or performing mathematical
     * transformations to normalize device data.</p> <p>The list can be 2-25
     * <code>PipelineActivity</code> objects and must contain both a
     * <code>channel</code> and a <code>datastore</code> activity. Each entry in the
     * list must contain only one activity. For example:</p> <p>
     * <code>pipelineActivities = [ { "channel": { ... } }, { "lambda": { ... } }, ...
     * ]</code> </p>
     */
    inline CreatePipelineRequest& WithPipelineActivities(const Aws::Vector<PipelineActivity>& value) { SetPipelineActivities(value); return *this;}

    /**
     * <p>A list of <code>PipelineActivity</code> objects. Activities perform
     * transformations on your messages, such as removing, renaming or adding message
     * attributes; filtering messages based on attribute values; invoking your Lambda
     * unctions on messages for advanced processing; or performing mathematical
     * transformations to normalize device data.</p> <p>The list can be 2-25
     * <code>PipelineActivity</code> objects and must contain both a
     * <code>channel</code> and a <code>datastore</code> activity. Each entry in the
     * list must contain only one activity. For example:</p> <p>
     * <code>pipelineActivities = [ { "channel": { ... } }, { "lambda": { ... } }, ...
     * ]</code> </p>
     */
    inline CreatePipelineRequest& WithPipelineActivities(Aws::Vector<PipelineActivity>&& value) { SetPipelineActivities(std::move(value)); return *this;}

    /**
     * <p>A list of <code>PipelineActivity</code> objects. Activities perform
     * transformations on your messages, such as removing, renaming or adding message
     * attributes; filtering messages based on attribute values; invoking your Lambda
     * unctions on messages for advanced processing; or performing mathematical
     * transformations to normalize device data.</p> <p>The list can be 2-25
     * <code>PipelineActivity</code> objects and must contain both a
     * <code>channel</code> and a <code>datastore</code> activity. Each entry in the
     * list must contain only one activity. For example:</p> <p>
     * <code>pipelineActivities = [ { "channel": { ... } }, { "lambda": { ... } }, ...
     * ]</code> </p>
     */
    inline CreatePipelineRequest& AddPipelineActivities(const PipelineActivity& value) { m_pipelineActivitiesHasBeenSet = true; m_pipelineActivities.push_back(value); return *this; }

    /**
     * <p>A list of <code>PipelineActivity</code> objects. Activities perform
     * transformations on your messages, such as removing, renaming or adding message
     * attributes; filtering messages based on attribute values; invoking your Lambda
     * unctions on messages for advanced processing; or performing mathematical
     * transformations to normalize device data.</p> <p>The list can be 2-25
     * <code>PipelineActivity</code> objects and must contain both a
     * <code>channel</code> and a <code>datastore</code> activity. Each entry in the
     * list must contain only one activity. For example:</p> <p>
     * <code>pipelineActivities = [ { "channel": { ... } }, { "lambda": { ... } }, ...
     * ]</code> </p>
     */
    inline CreatePipelineRequest& AddPipelineActivities(PipelineActivity&& value) { m_pipelineActivitiesHasBeenSet = true; m_pipelineActivities.push_back(std::move(value)); return *this; }


    /**
     * <p>Metadata which can be used to manage the pipeline.</p>
     */
    inline const Aws::Vector<Tag>& GetTags() const{ return m_tags; }

    /**
     * <p>Metadata which can be used to manage the pipeline.</p>
     */
    inline bool TagsHasBeenSet() const { return m_tagsHasBeenSet; }

    /**
     * <p>Metadata which can be used to manage the pipeline.</p>
     */
    inline void SetTags(const Aws::Vector<Tag>& value) { m_tagsHasBeenSet = true; m_tags = value; }

    /**
     * <p>Metadata which can be used to manage the pipeline.</p>
     */
    inline void SetTags(Aws::Vector<Tag>&& value) { m_tagsHasBeenSet = true; m_tags = std::move(value); }

    /**
     * <p>Metadata which can be used to manage the pipeline.</p>
     */
    inline CreatePipelineRequest& WithTags(const Aws::Vector<Tag>& value) { SetTags(value); return *this;}

    /**
     * <p>Metadata which can be used to manage the pipeline.</p>
     */
    inline CreatePipelineRequest& WithTags(Aws::Vector<Tag>&& value) { SetTags(std::move(value)); return *this;}

    /**
     * <p>Metadata which can be used to manage the pipeline.</p>
     */
    inline CreatePipelineRequest& AddTags(const Tag& value) { m_tagsHasBeenSet = true; m_tags.push_back(value); return *this; }

    /**
     * <p>Metadata which can be used to manage the pipeline.</p>
     */
    inline CreatePipelineRequest& AddTags(Tag&& value) { m_tagsHasBeenSet = true; m_tags.push_back(std::move(value)); return *this; }

  private:

    Aws::String m_pipelineName;
    bool m_pipelineNameHasBeenSet = false;

    Aws::Vector<PipelineActivity> m_pipelineActivities;
    bool m_pipelineActivitiesHasBeenSet = false;

    Aws::Vector<Tag> m_tags;
    bool m_tagsHasBeenSet = false;
  };

} // namespace Model
} // namespace IoTAnalytics
} // namespace Aws
