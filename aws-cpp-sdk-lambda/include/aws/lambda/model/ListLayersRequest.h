﻿/**
 * Copyright Amazon.com, Inc. or its affiliates. All Rights Reserved.
 * SPDX-License-Identifier: Apache-2.0.
 */

#pragma once
#include <aws/lambda/Lambda_EXPORTS.h>
#include <aws/lambda/LambdaRequest.h>
#include <aws/lambda/model/Runtime.h>
#include <aws/core/utils/memory/stl/AWSString.h>
#include <aws/lambda/model/Architecture.h>
#include <utility>

namespace Aws
{
namespace Http
{
    class URI;
} //namespace Http
namespace Lambda
{
namespace Model
{

  /**
   */
  class AWS_LAMBDA_API ListLayersRequest : public LambdaRequest
  {
  public:
    ListLayersRequest();

    /**
     * Virtual Copy Constructor idiom;
     * Override in an inherited class to have overridden functions (such as GetRequestSpecificHeaders) be called by the SDK.
     * (If you are creating a child of this: don't forget to keep the original return type or (better) just use the 'override' keyword)
    */
    virtual Aws::UniquePtr<ListLayersRequest> Clone() const
    {
      return Aws::MakeUnique<ListLayersRequest>(GetServiceRequestName(), *this);
    }

    // Service request name is the Operation name which will send this request out,
    // each operation should has unique request name, so that we can get operation's name from this request.
    // Note: this is not true for response, multiple operations may have the same response name,
    // so we can not get operation's name from response.
    inline virtual const char* GetServiceRequestName() const override { return "ListLayers"; }

    Aws::String SerializePayload() const override;

    void AddQueryStringParameters(Aws::Http::URI& uri) const override;


    /**
     * <p>A runtime identifier. For example, <code>go1.x</code>.</p>
     */
    inline const Runtime& GetCompatibleRuntime() const{ return m_compatibleRuntime; }

    /**
     * <p>A runtime identifier. For example, <code>go1.x</code>.</p>
     */
    inline bool CompatibleRuntimeHasBeenSet() const { return m_compatibleRuntimeHasBeenSet; }

    /**
     * <p>A runtime identifier. For example, <code>go1.x</code>.</p>
     */
    inline void SetCompatibleRuntime(const Runtime& value) { m_compatibleRuntimeHasBeenSet = true; m_compatibleRuntime = value; }

    /**
     * <p>A runtime identifier. For example, <code>go1.x</code>.</p>
     */
    inline void SetCompatibleRuntime(Runtime&& value) { m_compatibleRuntimeHasBeenSet = true; m_compatibleRuntime = std::move(value); }

    /**
     * <p>A runtime identifier. For example, <code>go1.x</code>.</p>
     */
    inline ListLayersRequest& WithCompatibleRuntime(const Runtime& value) { SetCompatibleRuntime(value); return *this;}

    /**
     * <p>A runtime identifier. For example, <code>go1.x</code>.</p>
     */
    inline ListLayersRequest& WithCompatibleRuntime(Runtime&& value) { SetCompatibleRuntime(std::move(value)); return *this;}


    /**
     * <p>A pagination token returned by a previous call.</p>
     */
    inline const Aws::String& GetMarker() const{ return m_marker; }

    /**
     * <p>A pagination token returned by a previous call.</p>
     */
    inline bool MarkerHasBeenSet() const { return m_markerHasBeenSet; }

    /**
     * <p>A pagination token returned by a previous call.</p>
     */
    inline void SetMarker(const Aws::String& value) { m_markerHasBeenSet = true; m_marker = value; }

    /**
     * <p>A pagination token returned by a previous call.</p>
     */
    inline void SetMarker(Aws::String&& value) { m_markerHasBeenSet = true; m_marker = std::move(value); }

    /**
     * <p>A pagination token returned by a previous call.</p>
     */
    inline void SetMarker(const char* value) { m_markerHasBeenSet = true; m_marker.assign(value); }

    /**
     * <p>A pagination token returned by a previous call.</p>
     */
    inline ListLayersRequest& WithMarker(const Aws::String& value) { SetMarker(value); return *this;}

    /**
     * <p>A pagination token returned by a previous call.</p>
     */
    inline ListLayersRequest& WithMarker(Aws::String&& value) { SetMarker(std::move(value)); return *this;}

    /**
     * <p>A pagination token returned by a previous call.</p>
     */
    inline ListLayersRequest& WithMarker(const char* value) { SetMarker(value); return *this;}


    /**
     * <p>The maximum number of layers to return.</p>
     */
    inline int GetMaxItems() const{ return m_maxItems; }

    /**
     * <p>The maximum number of layers to return.</p>
     */
    inline bool MaxItemsHasBeenSet() const { return m_maxItemsHasBeenSet; }

    /**
     * <p>The maximum number of layers to return.</p>
     */
    inline void SetMaxItems(int value) { m_maxItemsHasBeenSet = true; m_maxItems = value; }

    /**
     * <p>The maximum number of layers to return.</p>
     */
    inline ListLayersRequest& WithMaxItems(int value) { SetMaxItems(value); return *this;}


    /**
     * <p>The compatible <a
     * href="https://docs.aws.amazon.com/lambda/latest/dg/foundation-arch.html">instruction
     * set architecture</a>.</p>
     */
    inline const Architecture& GetCompatibleArchitecture() const{ return m_compatibleArchitecture; }

    /**
     * <p>The compatible <a
     * href="https://docs.aws.amazon.com/lambda/latest/dg/foundation-arch.html">instruction
     * set architecture</a>.</p>
     */
    inline bool CompatibleArchitectureHasBeenSet() const { return m_compatibleArchitectureHasBeenSet; }

    /**
     * <p>The compatible <a
     * href="https://docs.aws.amazon.com/lambda/latest/dg/foundation-arch.html">instruction
     * set architecture</a>.</p>
     */
    inline void SetCompatibleArchitecture(const Architecture& value) { m_compatibleArchitectureHasBeenSet = true; m_compatibleArchitecture = value; }

    /**
     * <p>The compatible <a
     * href="https://docs.aws.amazon.com/lambda/latest/dg/foundation-arch.html">instruction
     * set architecture</a>.</p>
     */
    inline void SetCompatibleArchitecture(Architecture&& value) { m_compatibleArchitectureHasBeenSet = true; m_compatibleArchitecture = std::move(value); }

    /**
     * <p>The compatible <a
     * href="https://docs.aws.amazon.com/lambda/latest/dg/foundation-arch.html">instruction
     * set architecture</a>.</p>
     */
    inline ListLayersRequest& WithCompatibleArchitecture(const Architecture& value) { SetCompatibleArchitecture(value); return *this;}

    /**
     * <p>The compatible <a
     * href="https://docs.aws.amazon.com/lambda/latest/dg/foundation-arch.html">instruction
     * set architecture</a>.</p>
     */
    inline ListLayersRequest& WithCompatibleArchitecture(Architecture&& value) { SetCompatibleArchitecture(std::move(value)); return *this;}

  private:

    Runtime m_compatibleRuntime;
    bool m_compatibleRuntimeHasBeenSet = false;

    Aws::String m_marker;
    bool m_markerHasBeenSet = false;

    int m_maxItems;
    bool m_maxItemsHasBeenSet = false;

    Architecture m_compatibleArchitecture;
    bool m_compatibleArchitectureHasBeenSet = false;
  };

} // namespace Model
} // namespace Lambda
} // namespace Aws
