﻿/**
 * Copyright Amazon.com, Inc. or its affiliates. All Rights Reserved.
 * SPDX-License-Identifier: Apache-2.0.
 */

#pragma once
#include <aws/route53resolver/Route53Resolver_EXPORTS.h>
#include <aws/route53resolver/Route53ResolverRequest.h>
#include <aws/core/utils/memory/stl/AWSString.h>
#include <utility>

namespace Aws
{
namespace Route53Resolver
{
namespace Model
{

  /**
   */
  class AWS_ROUTE53RESOLVER_API GetResolverQueryLogConfigPolicyRequest : public Route53ResolverRequest
  {
  public:
    GetResolverQueryLogConfigPolicyRequest();

    /**
     * Virtual Copy Constructor idiom;
     * Override in an inherited class to have overridden functions (such as GetRequestSpecificHeaders) be called by the SDK.
     * (If you are creating a child of this: don't forget to keep the original return type or (better) just use the 'override' keyword)
    */
    virtual Aws::UniquePtr<GetResolverQueryLogConfigPolicyRequest> Clone() const
    {
      return Aws::MakeUnique<GetResolverQueryLogConfigPolicyRequest>(GetServiceRequestName(), *this);
    }

    // Service request name is the Operation name which will send this request out,
    // each operation should has unique request name, so that we can get operation's name from this request.
    // Note: this is not true for response, multiple operations may have the same response name,
    // so we can not get operation's name from response.
    inline virtual const char* GetServiceRequestName() const override { return "GetResolverQueryLogConfigPolicy"; }

    Aws::String SerializePayload() const override;

    Aws::Http::HeaderValueCollection GetRequestSpecificHeaders() const override;


    /**
     * <p>The ARN of the query logging configuration that you want to get the query
     * logging policy for.</p>
     */
    inline const Aws::String& GetArn() const{ return m_arn; }

    /**
     * <p>The ARN of the query logging configuration that you want to get the query
     * logging policy for.</p>
     */
    inline bool ArnHasBeenSet() const { return m_arnHasBeenSet; }

    /**
     * <p>The ARN of the query logging configuration that you want to get the query
     * logging policy for.</p>
     */
    inline void SetArn(const Aws::String& value) { m_arnHasBeenSet = true; m_arn = value; }

    /**
     * <p>The ARN of the query logging configuration that you want to get the query
     * logging policy for.</p>
     */
    inline void SetArn(Aws::String&& value) { m_arnHasBeenSet = true; m_arn = std::move(value); }

    /**
     * <p>The ARN of the query logging configuration that you want to get the query
     * logging policy for.</p>
     */
    inline void SetArn(const char* value) { m_arnHasBeenSet = true; m_arn.assign(value); }

    /**
     * <p>The ARN of the query logging configuration that you want to get the query
     * logging policy for.</p>
     */
    inline GetResolverQueryLogConfigPolicyRequest& WithArn(const Aws::String& value) { SetArn(value); return *this;}

    /**
     * <p>The ARN of the query logging configuration that you want to get the query
     * logging policy for.</p>
     */
    inline GetResolverQueryLogConfigPolicyRequest& WithArn(Aws::String&& value) { SetArn(std::move(value)); return *this;}

    /**
     * <p>The ARN of the query logging configuration that you want to get the query
     * logging policy for.</p>
     */
    inline GetResolverQueryLogConfigPolicyRequest& WithArn(const char* value) { SetArn(value); return *this;}

  private:

    Aws::String m_arn;
    bool m_arnHasBeenSet = false;
  };

} // namespace Model
} // namespace Route53Resolver
} // namespace Aws
