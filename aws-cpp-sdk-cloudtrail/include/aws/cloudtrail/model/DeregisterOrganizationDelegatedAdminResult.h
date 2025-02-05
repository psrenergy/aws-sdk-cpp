﻿/**
 * Copyright Amazon.com, Inc. or its affiliates. All Rights Reserved.
 * SPDX-License-Identifier: Apache-2.0.
 */

#pragma once
#include <aws/cloudtrail/CloudTrail_EXPORTS.h>

namespace Aws
{
template<typename RESULT_TYPE>
class AmazonWebServiceResult;

namespace Utils
{
namespace Json
{
  class JsonValue;
} // namespace Json
} // namespace Utils
namespace CloudTrail
{
namespace Model
{
  /**
   * <p>Returns the following response if successful. Otherwise, returns an
   * error.</p><p><h3>See Also:</h3>   <a
   * href="http://docs.aws.amazon.com/goto/WebAPI/cloudtrail-2013-11-01/DeregisterOrganizationDelegatedAdminResponse">AWS
   * API Reference</a></p>
   */
  class AWS_CLOUDTRAIL_API DeregisterOrganizationDelegatedAdminResult
  {
  public:
    DeregisterOrganizationDelegatedAdminResult();
    DeregisterOrganizationDelegatedAdminResult(const Aws::AmazonWebServiceResult<Aws::Utils::Json::JsonValue>& result);
    DeregisterOrganizationDelegatedAdminResult& operator=(const Aws::AmazonWebServiceResult<Aws::Utils::Json::JsonValue>& result);

  };

} // namespace Model
} // namespace CloudTrail
} // namespace Aws
