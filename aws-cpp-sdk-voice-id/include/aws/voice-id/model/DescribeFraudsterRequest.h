﻿/**
 * Copyright Amazon.com, Inc. or its affiliates. All Rights Reserved.
 * SPDX-License-Identifier: Apache-2.0.
 */

#pragma once
#include <aws/voice-id/VoiceID_EXPORTS.h>
#include <aws/voice-id/VoiceIDRequest.h>
#include <aws/core/utils/memory/stl/AWSString.h>
#include <utility>

namespace Aws
{
namespace VoiceID
{
namespace Model
{

  /**
   */
  class AWS_VOICEID_API DescribeFraudsterRequest : public VoiceIDRequest
  {
  public:
    DescribeFraudsterRequest();

    /**
     * Virtual Copy Constructor idiom;
     * Override in an inherited class to have overridden functions (such as GetRequestSpecificHeaders) be called by the SDK.
     * (If you are creating a child of this: don't forget to keep the original return type or (better) just use the 'override' keyword)
    */
    virtual Aws::UniquePtr<DescribeFraudsterRequest> Clone() const
    {
      return Aws::MakeUnique<DescribeFraudsterRequest>(GetServiceRequestName(), *this);
    }

    // Service request name is the Operation name which will send this request out,
    // each operation should has unique request name, so that we can get operation's name from this request.
    // Note: this is not true for response, multiple operations may have the same response name,
    // so we can not get operation's name from response.
    inline virtual const char* GetServiceRequestName() const override { return "DescribeFraudster"; }

    Aws::String SerializePayload() const override;

    Aws::Http::HeaderValueCollection GetRequestSpecificHeaders() const override;


    /**
     * <p>The identifier of the domain containing the fraudster.</p>
     */
    inline const Aws::String& GetDomainId() const{ return m_domainId; }

    /**
     * <p>The identifier of the domain containing the fraudster.</p>
     */
    inline bool DomainIdHasBeenSet() const { return m_domainIdHasBeenSet; }

    /**
     * <p>The identifier of the domain containing the fraudster.</p>
     */
    inline void SetDomainId(const Aws::String& value) { m_domainIdHasBeenSet = true; m_domainId = value; }

    /**
     * <p>The identifier of the domain containing the fraudster.</p>
     */
    inline void SetDomainId(Aws::String&& value) { m_domainIdHasBeenSet = true; m_domainId = std::move(value); }

    /**
     * <p>The identifier of the domain containing the fraudster.</p>
     */
    inline void SetDomainId(const char* value) { m_domainIdHasBeenSet = true; m_domainId.assign(value); }

    /**
     * <p>The identifier of the domain containing the fraudster.</p>
     */
    inline DescribeFraudsterRequest& WithDomainId(const Aws::String& value) { SetDomainId(value); return *this;}

    /**
     * <p>The identifier of the domain containing the fraudster.</p>
     */
    inline DescribeFraudsterRequest& WithDomainId(Aws::String&& value) { SetDomainId(std::move(value)); return *this;}

    /**
     * <p>The identifier of the domain containing the fraudster.</p>
     */
    inline DescribeFraudsterRequest& WithDomainId(const char* value) { SetDomainId(value); return *this;}


    /**
     * <p>The identifier of the fraudster you are describing.</p>
     */
    inline const Aws::String& GetFraudsterId() const{ return m_fraudsterId; }

    /**
     * <p>The identifier of the fraudster you are describing.</p>
     */
    inline bool FraudsterIdHasBeenSet() const { return m_fraudsterIdHasBeenSet; }

    /**
     * <p>The identifier of the fraudster you are describing.</p>
     */
    inline void SetFraudsterId(const Aws::String& value) { m_fraudsterIdHasBeenSet = true; m_fraudsterId = value; }

    /**
     * <p>The identifier of the fraudster you are describing.</p>
     */
    inline void SetFraudsterId(Aws::String&& value) { m_fraudsterIdHasBeenSet = true; m_fraudsterId = std::move(value); }

    /**
     * <p>The identifier of the fraudster you are describing.</p>
     */
    inline void SetFraudsterId(const char* value) { m_fraudsterIdHasBeenSet = true; m_fraudsterId.assign(value); }

    /**
     * <p>The identifier of the fraudster you are describing.</p>
     */
    inline DescribeFraudsterRequest& WithFraudsterId(const Aws::String& value) { SetFraudsterId(value); return *this;}

    /**
     * <p>The identifier of the fraudster you are describing.</p>
     */
    inline DescribeFraudsterRequest& WithFraudsterId(Aws::String&& value) { SetFraudsterId(std::move(value)); return *this;}

    /**
     * <p>The identifier of the fraudster you are describing.</p>
     */
    inline DescribeFraudsterRequest& WithFraudsterId(const char* value) { SetFraudsterId(value); return *this;}

  private:

    Aws::String m_domainId;
    bool m_domainIdHasBeenSet = false;

    Aws::String m_fraudsterId;
    bool m_fraudsterIdHasBeenSet = false;
  };

} // namespace Model
} // namespace VoiceID
} // namespace Aws
