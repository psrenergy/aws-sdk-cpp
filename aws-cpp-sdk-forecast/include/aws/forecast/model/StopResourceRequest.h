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
  class AWS_FORECASTSERVICE_API StopResourceRequest : public ForecastServiceRequest
  {
  public:
    StopResourceRequest();

    // Service request name is the Operation name which will send this request out,
    // each operation should has unique request name, so that we can get operation's name from this request.
    // Note: this is not true for response, multiple operations may have the same response name,
    // so we can not get operation's name from response.
    inline virtual const char* GetServiceRequestName() const override { return "StopResource"; }

    Aws::String SerializePayload() const override;

    Aws::Http::HeaderValueCollection GetRequestSpecificHeaders() const override;


    /**
     * <p>The Amazon Resource Name (ARN) that identifies the resource to stop. The
     * supported ARNs are <code>DatasetImportJobArn</code>, <code>PredictorArn</code>,
     * <code>PredictorBacktestExportJobArn</code>, <code>ForecastArn</code>,
     * <code>ForecastExportJobArn</code>, <code>ExplainabilityArn</code>, and
     * <code>ExplainabilityExportArn</code>. </p>
     */
    inline const Aws::String& GetResourceArn() const{ return m_resourceArn; }

    /**
     * <p>The Amazon Resource Name (ARN) that identifies the resource to stop. The
     * supported ARNs are <code>DatasetImportJobArn</code>, <code>PredictorArn</code>,
     * <code>PredictorBacktestExportJobArn</code>, <code>ForecastArn</code>,
     * <code>ForecastExportJobArn</code>, <code>ExplainabilityArn</code>, and
     * <code>ExplainabilityExportArn</code>. </p>
     */
    inline bool ResourceArnHasBeenSet() const { return m_resourceArnHasBeenSet; }

    /**
     * <p>The Amazon Resource Name (ARN) that identifies the resource to stop. The
     * supported ARNs are <code>DatasetImportJobArn</code>, <code>PredictorArn</code>,
     * <code>PredictorBacktestExportJobArn</code>, <code>ForecastArn</code>,
     * <code>ForecastExportJobArn</code>, <code>ExplainabilityArn</code>, and
     * <code>ExplainabilityExportArn</code>. </p>
     */
    inline void SetResourceArn(const Aws::String& value) { m_resourceArnHasBeenSet = true; m_resourceArn = value; }

    /**
     * <p>The Amazon Resource Name (ARN) that identifies the resource to stop. The
     * supported ARNs are <code>DatasetImportJobArn</code>, <code>PredictorArn</code>,
     * <code>PredictorBacktestExportJobArn</code>, <code>ForecastArn</code>,
     * <code>ForecastExportJobArn</code>, <code>ExplainabilityArn</code>, and
     * <code>ExplainabilityExportArn</code>. </p>
     */
    inline void SetResourceArn(Aws::String&& value) { m_resourceArnHasBeenSet = true; m_resourceArn = std::move(value); }

    /**
     * <p>The Amazon Resource Name (ARN) that identifies the resource to stop. The
     * supported ARNs are <code>DatasetImportJobArn</code>, <code>PredictorArn</code>,
     * <code>PredictorBacktestExportJobArn</code>, <code>ForecastArn</code>,
     * <code>ForecastExportJobArn</code>, <code>ExplainabilityArn</code>, and
     * <code>ExplainabilityExportArn</code>. </p>
     */
    inline void SetResourceArn(const char* value) { m_resourceArnHasBeenSet = true; m_resourceArn.assign(value); }

    /**
     * <p>The Amazon Resource Name (ARN) that identifies the resource to stop. The
     * supported ARNs are <code>DatasetImportJobArn</code>, <code>PredictorArn</code>,
     * <code>PredictorBacktestExportJobArn</code>, <code>ForecastArn</code>,
     * <code>ForecastExportJobArn</code>, <code>ExplainabilityArn</code>, and
     * <code>ExplainabilityExportArn</code>. </p>
     */
    inline StopResourceRequest& WithResourceArn(const Aws::String& value) { SetResourceArn(value); return *this;}

    /**
     * <p>The Amazon Resource Name (ARN) that identifies the resource to stop. The
     * supported ARNs are <code>DatasetImportJobArn</code>, <code>PredictorArn</code>,
     * <code>PredictorBacktestExportJobArn</code>, <code>ForecastArn</code>,
     * <code>ForecastExportJobArn</code>, <code>ExplainabilityArn</code>, and
     * <code>ExplainabilityExportArn</code>. </p>
     */
    inline StopResourceRequest& WithResourceArn(Aws::String&& value) { SetResourceArn(std::move(value)); return *this;}

    /**
     * <p>The Amazon Resource Name (ARN) that identifies the resource to stop. The
     * supported ARNs are <code>DatasetImportJobArn</code>, <code>PredictorArn</code>,
     * <code>PredictorBacktestExportJobArn</code>, <code>ForecastArn</code>,
     * <code>ForecastExportJobArn</code>, <code>ExplainabilityArn</code>, and
     * <code>ExplainabilityExportArn</code>. </p>
     */
    inline StopResourceRequest& WithResourceArn(const char* value) { SetResourceArn(value); return *this;}

  private:

    Aws::String m_resourceArn;
    bool m_resourceArnHasBeenSet = false;
  };

} // namespace Model
} // namespace ForecastService
} // namespace Aws
