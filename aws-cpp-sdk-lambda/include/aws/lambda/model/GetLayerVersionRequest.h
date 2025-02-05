﻿/**
 * Copyright Amazon.com, Inc. or its affiliates. All Rights Reserved.
 * SPDX-License-Identifier: Apache-2.0.
 */

#pragma once
#include <aws/lambda/Lambda_EXPORTS.h>
#include <aws/lambda/LambdaRequest.h>
#include <aws/core/utils/memory/stl/AWSString.h>
#include <utility>

namespace Aws
{
namespace Lambda
{
namespace Model
{

  /**
   */
  class AWS_LAMBDA_API GetLayerVersionRequest : public LambdaRequest
  {
  public:
    GetLayerVersionRequest();

    // Service request name is the Operation name which will send this request out,
    // each operation should has unique request name, so that we can get operation's name from this request.
    // Note: this is not true for response, multiple operations may have the same response name,
    // so we can not get operation's name from response.
    inline virtual const char* GetServiceRequestName() const override { return "GetLayerVersion"; }

    Aws::String SerializePayload() const override;


    /**
     * <p>The name or Amazon Resource Name (ARN) of the layer.</p>
     */
    inline const Aws::String& GetLayerName() const{ return m_layerName; }

    /**
     * <p>The name or Amazon Resource Name (ARN) of the layer.</p>
     */
    inline bool LayerNameHasBeenSet() const { return m_layerNameHasBeenSet; }

    /**
     * <p>The name or Amazon Resource Name (ARN) of the layer.</p>
     */
    inline void SetLayerName(const Aws::String& value) { m_layerNameHasBeenSet = true; m_layerName = value; }

    /**
     * <p>The name or Amazon Resource Name (ARN) of the layer.</p>
     */
    inline void SetLayerName(Aws::String&& value) { m_layerNameHasBeenSet = true; m_layerName = std::move(value); }

    /**
     * <p>The name or Amazon Resource Name (ARN) of the layer.</p>
     */
    inline void SetLayerName(const char* value) { m_layerNameHasBeenSet = true; m_layerName.assign(value); }

    /**
     * <p>The name or Amazon Resource Name (ARN) of the layer.</p>
     */
    inline GetLayerVersionRequest& WithLayerName(const Aws::String& value) { SetLayerName(value); return *this;}

    /**
     * <p>The name or Amazon Resource Name (ARN) of the layer.</p>
     */
    inline GetLayerVersionRequest& WithLayerName(Aws::String&& value) { SetLayerName(std::move(value)); return *this;}

    /**
     * <p>The name or Amazon Resource Name (ARN) of the layer.</p>
     */
    inline GetLayerVersionRequest& WithLayerName(const char* value) { SetLayerName(value); return *this;}


    /**
     * <p>The version number.</p>
     */
    inline long long GetVersionNumber() const{ return m_versionNumber; }

    /**
     * <p>The version number.</p>
     */
    inline bool VersionNumberHasBeenSet() const { return m_versionNumberHasBeenSet; }

    /**
     * <p>The version number.</p>
     */
    inline void SetVersionNumber(long long value) { m_versionNumberHasBeenSet = true; m_versionNumber = value; }

    /**
     * <p>The version number.</p>
     */
    inline GetLayerVersionRequest& WithVersionNumber(long long value) { SetVersionNumber(value); return *this;}

  private:

    Aws::String m_layerName;
    bool m_layerNameHasBeenSet = false;

    long long m_versionNumber;
    bool m_versionNumberHasBeenSet = false;
  };

} // namespace Model
} // namespace Lambda
} // namespace Aws
