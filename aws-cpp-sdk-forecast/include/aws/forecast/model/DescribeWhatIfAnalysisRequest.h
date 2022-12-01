﻿/**
 * Copyright Amazon.com, Inc. or its affiliates. All Rights Reserved.
 * SPDX-License-Identifier: Apache-2.0.
 */

#pragma once
#include <aws/forecast/ForecastService_EXPORTS.h>
#include <aws/forecast/ForecastServiceRequest.h>
#include <aws/core/utils/memory/stl/AWSString.h>
#include <utility>

namespace Aws
{
namespace ForecastService
{
namespace Model
{

  /**
   */
  class AWS_FORECASTSERVICE_API DescribeWhatIfAnalysisRequest : public ForecastServiceRequest
  {
  public:
    DescribeWhatIfAnalysisRequest();

    /**
     * Virtual Copy Constructor idiom;
     * Override in an inherited class to have overridden functions (such as GetRequestSpecificHeaders) be called by the SDK.
     * (If you are creating a child of this: don't forget to keep the original return type or (better) just use the 'override' keyword)
    */
    virtual Aws::UniquePtr<DescribeWhatIfAnalysisRequest> Clone() const
    {
      return Aws::MakeUnique<DescribeWhatIfAnalysisRequest>(GetServiceRequestName(), *this);
    }

    // Service request name is the Operation name which will send this request out,
    // each operation should has unique request name, so that we can get operation's name from this request.
    // Note: this is not true for response, multiple operations may have the same response name,
    // so we can not get operation's name from response.
    inline virtual const char* GetServiceRequestName() const override { return "DescribeWhatIfAnalysis"; }

    Aws::String SerializePayload() const override;

    Aws::Http::HeaderValueCollection GetRequestSpecificHeaders() const override;


    /**
     * <p>The Amazon Resource Name (ARN) of the what-if analysis that you are
     * interested in.</p>
     */
    inline const Aws::String& GetWhatIfAnalysisArn() const{ return m_whatIfAnalysisArn; }

    /**
     * <p>The Amazon Resource Name (ARN) of the what-if analysis that you are
     * interested in.</p>
     */
    inline bool WhatIfAnalysisArnHasBeenSet() const { return m_whatIfAnalysisArnHasBeenSet; }

    /**
     * <p>The Amazon Resource Name (ARN) of the what-if analysis that you are
     * interested in.</p>
     */
    inline void SetWhatIfAnalysisArn(const Aws::String& value) { m_whatIfAnalysisArnHasBeenSet = true; m_whatIfAnalysisArn = value; }

    /**
     * <p>The Amazon Resource Name (ARN) of the what-if analysis that you are
     * interested in.</p>
     */
    inline void SetWhatIfAnalysisArn(Aws::String&& value) { m_whatIfAnalysisArnHasBeenSet = true; m_whatIfAnalysisArn = std::move(value); }

    /**
     * <p>The Amazon Resource Name (ARN) of the what-if analysis that you are
     * interested in.</p>
     */
    inline void SetWhatIfAnalysisArn(const char* value) { m_whatIfAnalysisArnHasBeenSet = true; m_whatIfAnalysisArn.assign(value); }

    /**
     * <p>The Amazon Resource Name (ARN) of the what-if analysis that you are
     * interested in.</p>
     */
    inline DescribeWhatIfAnalysisRequest& WithWhatIfAnalysisArn(const Aws::String& value) { SetWhatIfAnalysisArn(value); return *this;}

    /**
     * <p>The Amazon Resource Name (ARN) of the what-if analysis that you are
     * interested in.</p>
     */
    inline DescribeWhatIfAnalysisRequest& WithWhatIfAnalysisArn(Aws::String&& value) { SetWhatIfAnalysisArn(std::move(value)); return *this;}

    /**
     * <p>The Amazon Resource Name (ARN) of the what-if analysis that you are
     * interested in.</p>
     */
    inline DescribeWhatIfAnalysisRequest& WithWhatIfAnalysisArn(const char* value) { SetWhatIfAnalysisArn(value); return *this;}

  private:

    Aws::String m_whatIfAnalysisArn;
    bool m_whatIfAnalysisArnHasBeenSet = false;
  };

} // namespace Model
} // namespace ForecastService
} // namespace Aws
