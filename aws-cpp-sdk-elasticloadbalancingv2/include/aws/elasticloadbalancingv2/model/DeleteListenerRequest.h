﻿/**
 * Copyright Amazon.com, Inc. or its affiliates. All Rights Reserved.
 * SPDX-License-Identifier: Apache-2.0.
 */

#pragma once
#include <aws/elasticloadbalancingv2/ElasticLoadBalancingv2_EXPORTS.h>
#include <aws/elasticloadbalancingv2/ElasticLoadBalancingv2Request.h>
#include <aws/core/utils/memory/stl/AWSString.h>
#include <utility>

namespace Aws
{
namespace ElasticLoadBalancingv2
{
namespace Model
{

  /**
   */
  class AWS_ELASTICLOADBALANCINGV2_API DeleteListenerRequest : public ElasticLoadBalancingv2Request
  {
  public:
    DeleteListenerRequest();

    /**
     * Virtual Copy Constructor idiom;
     * Override in an inherited class to have overridden functions (such as GetRequestSpecificHeaders) be called by the SDK.
     * (If you are creating a child of this: don't forget to keep the original return type or (better) just use the 'override' keyword)
    */
    virtual Aws::UniquePtr<DeleteListenerRequest> Clone() const
    {
      return Aws::MakeUnique<DeleteListenerRequest>(GetServiceRequestName(), *this);
    }

    // Service request name is the Operation name which will send this request out,
    // each operation should has unique request name, so that we can get operation's name from this request.
    // Note: this is not true for response, multiple operations may have the same response name,
    // so we can not get operation's name from response.
    inline virtual const char* GetServiceRequestName() const override { return "DeleteListener"; }

    Aws::String SerializePayload() const override;

  protected:
    void DumpBodyToUrl(Aws::Http::URI& uri ) const override;

  public:

    /**
     * <p>The Amazon Resource Name (ARN) of the listener.</p>
     */
    inline const Aws::String& GetListenerArn() const{ return m_listenerArn; }

    /**
     * <p>The Amazon Resource Name (ARN) of the listener.</p>
     */
    inline bool ListenerArnHasBeenSet() const { return m_listenerArnHasBeenSet; }

    /**
     * <p>The Amazon Resource Name (ARN) of the listener.</p>
     */
    inline void SetListenerArn(const Aws::String& value) { m_listenerArnHasBeenSet = true; m_listenerArn = value; }

    /**
     * <p>The Amazon Resource Name (ARN) of the listener.</p>
     */
    inline void SetListenerArn(Aws::String&& value) { m_listenerArnHasBeenSet = true; m_listenerArn = std::move(value); }

    /**
     * <p>The Amazon Resource Name (ARN) of the listener.</p>
     */
    inline void SetListenerArn(const char* value) { m_listenerArnHasBeenSet = true; m_listenerArn.assign(value); }

    /**
     * <p>The Amazon Resource Name (ARN) of the listener.</p>
     */
    inline DeleteListenerRequest& WithListenerArn(const Aws::String& value) { SetListenerArn(value); return *this;}

    /**
     * <p>The Amazon Resource Name (ARN) of the listener.</p>
     */
    inline DeleteListenerRequest& WithListenerArn(Aws::String&& value) { SetListenerArn(std::move(value)); return *this;}

    /**
     * <p>The Amazon Resource Name (ARN) of the listener.</p>
     */
    inline DeleteListenerRequest& WithListenerArn(const char* value) { SetListenerArn(value); return *this;}

  private:

    Aws::String m_listenerArn;
    bool m_listenerArnHasBeenSet = false;
  };

} // namespace Model
} // namespace ElasticLoadBalancingv2
} // namespace Aws
