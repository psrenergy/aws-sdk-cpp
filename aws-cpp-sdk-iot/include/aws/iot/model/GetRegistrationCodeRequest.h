﻿/**
 * Copyright Amazon.com, Inc. or its affiliates. All Rights Reserved.
 * SPDX-License-Identifier: Apache-2.0.
 */

#pragma once
#include <aws/iot/IoT_EXPORTS.h>
#include <aws/iot/IoTRequest.h>

namespace Aws
{
namespace IoT
{
namespace Model
{

  /**
   * <p>The input to the GetRegistrationCode operation.</p><p><h3>See Also:</h3>   <a
   * href="http://docs.aws.amazon.com/goto/WebAPI/iot-2015-05-28/GetRegistrationCodeRequest">AWS
   * API Reference</a></p>
   */
  class AWS_IOT_API GetRegistrationCodeRequest : public IoTRequest
  {
  public:
    GetRegistrationCodeRequest();

    /**
     * Virtual Copy Constructor idiom;
     * Override in an inherited class to have overridden functions (such as GetRequestSpecificHeaders) be called by the SDK.
     * (If you are creating a child of this: don't forget to keep the original return type or (better) just use the 'override' keyword)
    */
    virtual Aws::UniquePtr<GetRegistrationCodeRequest> Clone() const
    {
      return Aws::MakeUnique<GetRegistrationCodeRequest>(GetServiceRequestName(), *this);
    }

    // Service request name is the Operation name which will send this request out,
    // each operation should has unique request name, so that we can get operation's name from this request.
    // Note: this is not true for response, multiple operations may have the same response name,
    // so we can not get operation's name from response.
    inline virtual const char* GetServiceRequestName() const override { return "GetRegistrationCode"; }

    Aws::String SerializePayload() const override;

  };

} // namespace Model
} // namespace IoT
} // namespace Aws
