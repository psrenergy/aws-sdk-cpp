﻿/**
 * Copyright Amazon.com, Inc. or its affiliates. All Rights Reserved.
 * SPDX-License-Identifier: Apache-2.0.
 */

#pragma once
#include <aws/opensearch/OpenSearchService_EXPORTS.h>
#include <aws/opensearch/OpenSearchServiceRequest.h>
#include <aws/core/utils/memory/stl/AWSString.h>
#include <utility>

namespace Aws
{
namespace OpenSearchService
{
namespace Model
{

  /**
   * <p>Container for the request parameters to cancel a service software
   * update.</p><p><h3>See Also:</h3>   <a
   * href="http://docs.aws.amazon.com/goto/WebAPI/opensearch-2021-01-01/CancelServiceSoftwareUpdateRequest">AWS
   * API Reference</a></p>
   */
  class AWS_OPENSEARCHSERVICE_API CancelServiceSoftwareUpdateRequest : public OpenSearchServiceRequest
  {
  public:
    CancelServiceSoftwareUpdateRequest();

    /**
     * Virtual Copy Constructor idiom;
     * Override in an inherited class to have overridden functions (such as GetRequestSpecificHeaders) be called by the SDK.
     * (If you are creating a child of this: don't forget to keep the original return type or (better) just use the 'override' keyword)
    */
    virtual Aws::UniquePtr<CancelServiceSoftwareUpdateRequest> Clone() const
    {
      return Aws::MakeUnique<CancelServiceSoftwareUpdateRequest>(GetServiceRequestName(), *this);
    }

    // Service request name is the Operation name which will send this request out,
    // each operation should has unique request name, so that we can get operation's name from this request.
    // Note: this is not true for response, multiple operations may have the same response name,
    // so we can not get operation's name from response.
    inline virtual const char* GetServiceRequestName() const override { return "CancelServiceSoftwareUpdate"; }

    Aws::String SerializePayload() const override;


    /**
     * <p>Name of the OpenSearch Service domain that you want to cancel the service
     * software update on.</p>
     */
    inline const Aws::String& GetDomainName() const{ return m_domainName; }

    /**
     * <p>Name of the OpenSearch Service domain that you want to cancel the service
     * software update on.</p>
     */
    inline bool DomainNameHasBeenSet() const { return m_domainNameHasBeenSet; }

    /**
     * <p>Name of the OpenSearch Service domain that you want to cancel the service
     * software update on.</p>
     */
    inline void SetDomainName(const Aws::String& value) { m_domainNameHasBeenSet = true; m_domainName = value; }

    /**
     * <p>Name of the OpenSearch Service domain that you want to cancel the service
     * software update on.</p>
     */
    inline void SetDomainName(Aws::String&& value) { m_domainNameHasBeenSet = true; m_domainName = std::move(value); }

    /**
     * <p>Name of the OpenSearch Service domain that you want to cancel the service
     * software update on.</p>
     */
    inline void SetDomainName(const char* value) { m_domainNameHasBeenSet = true; m_domainName.assign(value); }

    /**
     * <p>Name of the OpenSearch Service domain that you want to cancel the service
     * software update on.</p>
     */
    inline CancelServiceSoftwareUpdateRequest& WithDomainName(const Aws::String& value) { SetDomainName(value); return *this;}

    /**
     * <p>Name of the OpenSearch Service domain that you want to cancel the service
     * software update on.</p>
     */
    inline CancelServiceSoftwareUpdateRequest& WithDomainName(Aws::String&& value) { SetDomainName(std::move(value)); return *this;}

    /**
     * <p>Name of the OpenSearch Service domain that you want to cancel the service
     * software update on.</p>
     */
    inline CancelServiceSoftwareUpdateRequest& WithDomainName(const char* value) { SetDomainName(value); return *this;}

  private:

    Aws::String m_domainName;
    bool m_domainNameHasBeenSet = false;
  };

} // namespace Model
} // namespace OpenSearchService
} // namespace Aws
