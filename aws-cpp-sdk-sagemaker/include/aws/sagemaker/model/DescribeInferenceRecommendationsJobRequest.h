﻿/**
 * Copyright Amazon.com, Inc. or its affiliates. All Rights Reserved.
 * SPDX-License-Identifier: Apache-2.0.
 */

#pragma once
#include <aws/sagemaker/SageMaker_EXPORTS.h>
#include <aws/sagemaker/SageMakerRequest.h>
#include <aws/core/utils/memory/stl/AWSString.h>
#include <utility>

namespace Aws
{
namespace SageMaker
{
namespace Model
{

  /**
   */
  class AWS_SAGEMAKER_API DescribeInferenceRecommendationsJobRequest : public SageMakerRequest
  {
  public:
    DescribeInferenceRecommendationsJobRequest();

    /**
     * Virtual Copy Constructor idiom;
     * Override in an inherited class to have overridden functions (such as GetRequestSpecificHeaders) be called by the SDK.
     * (If you are creating a child of this: don't forget to keep the original return type or (better) just use the 'override' keyword)
    */
    virtual Aws::UniquePtr<DescribeInferenceRecommendationsJobRequest> Clone() const
    {
      return Aws::MakeUnique<DescribeInferenceRecommendationsJobRequest>(GetServiceRequestName(), *this);
    }

    // Service request name is the Operation name which will send this request out,
    // each operation should has unique request name, so that we can get operation's name from this request.
    // Note: this is not true for response, multiple operations may have the same response name,
    // so we can not get operation's name from response.
    inline virtual const char* GetServiceRequestName() const override { return "DescribeInferenceRecommendationsJob"; }

    Aws::String SerializePayload() const override;

    Aws::Http::HeaderValueCollection GetRequestSpecificHeaders() const override;


    /**
     * <p>The name of the job. The name must be unique within an Amazon Web Services
     * Region in the Amazon Web Services account.</p>
     */
    inline const Aws::String& GetJobName() const{ return m_jobName; }

    /**
     * <p>The name of the job. The name must be unique within an Amazon Web Services
     * Region in the Amazon Web Services account.</p>
     */
    inline bool JobNameHasBeenSet() const { return m_jobNameHasBeenSet; }

    /**
     * <p>The name of the job. The name must be unique within an Amazon Web Services
     * Region in the Amazon Web Services account.</p>
     */
    inline void SetJobName(const Aws::String& value) { m_jobNameHasBeenSet = true; m_jobName = value; }

    /**
     * <p>The name of the job. The name must be unique within an Amazon Web Services
     * Region in the Amazon Web Services account.</p>
     */
    inline void SetJobName(Aws::String&& value) { m_jobNameHasBeenSet = true; m_jobName = std::move(value); }

    /**
     * <p>The name of the job. The name must be unique within an Amazon Web Services
     * Region in the Amazon Web Services account.</p>
     */
    inline void SetJobName(const char* value) { m_jobNameHasBeenSet = true; m_jobName.assign(value); }

    /**
     * <p>The name of the job. The name must be unique within an Amazon Web Services
     * Region in the Amazon Web Services account.</p>
     */
    inline DescribeInferenceRecommendationsJobRequest& WithJobName(const Aws::String& value) { SetJobName(value); return *this;}

    /**
     * <p>The name of the job. The name must be unique within an Amazon Web Services
     * Region in the Amazon Web Services account.</p>
     */
    inline DescribeInferenceRecommendationsJobRequest& WithJobName(Aws::String&& value) { SetJobName(std::move(value)); return *this;}

    /**
     * <p>The name of the job. The name must be unique within an Amazon Web Services
     * Region in the Amazon Web Services account.</p>
     */
    inline DescribeInferenceRecommendationsJobRequest& WithJobName(const char* value) { SetJobName(value); return *this;}

  private:

    Aws::String m_jobName;
    bool m_jobNameHasBeenSet = false;
  };

} // namespace Model
} // namespace SageMaker
} // namespace Aws
