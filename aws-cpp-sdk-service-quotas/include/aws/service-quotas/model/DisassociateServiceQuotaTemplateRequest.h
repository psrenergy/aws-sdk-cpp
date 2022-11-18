﻿/**
 * Copyright Amazon.com, Inc. or its affiliates. All Rights Reserved.
 * SPDX-License-Identifier: Apache-2.0.
 */

#pragma once
#include <aws/service-quotas/ServiceQuotas_EXPORTS.h>
#include <aws/service-quotas/ServiceQuotasRequest.h>

namespace Aws
{
namespace ServiceQuotas
{
namespace Model
{

  /**
   */
  class AWS_SERVICEQUOTAS_API DisassociateServiceQuotaTemplateRequest : public ServiceQuotasRequest
  {
  public:
    DisassociateServiceQuotaTemplateRequest();

    /**
     * Virtual Copy Constructor idiom;
     * Override in an inherited class to have overridden functions (such as GetRequestSpecificHeaders) be called by the SDK.
     * (If you are creating a child of this: don't forget to keep the original return type or (better) just use the 'override' keyword)
    */
    virtual Aws::UniquePtr<DisassociateServiceQuotaTemplateRequest> Clone() const
    {
      return Aws::MakeUnique<DisassociateServiceQuotaTemplateRequest>(GetServiceRequestName(), *this);
    }

    // Service request name is the Operation name which will send this request out,
    // each operation should has unique request name, so that we can get operation's name from this request.
    // Note: this is not true for response, multiple operations may have the same response name,
    // so we can not get operation's name from response.
    inline virtual const char* GetServiceRequestName() const override { return "DisassociateServiceQuotaTemplate"; }

    Aws::String SerializePayload() const override;

    Aws::Http::HeaderValueCollection GetRequestSpecificHeaders() const override;

  };

} // namespace Model
} // namespace ServiceQuotas
} // namespace Aws
