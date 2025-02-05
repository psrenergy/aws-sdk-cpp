﻿/**
 * Copyright Amazon.com, Inc. or its affiliates. All Rights Reserved.
 * SPDX-License-Identifier: Apache-2.0.
 */

#pragma once
#include <aws/es/ElasticsearchService_EXPORTS.h>
#include <aws/es/model/PrincipalType.h>
#include <aws/core/utils/memory/stl/AWSString.h>
#include <utility>

namespace Aws
{
namespace Utils
{
namespace Json
{
  class JsonValue;
  class JsonView;
} // namespace Json
} // namespace Utils
namespace ElasticsearchService
{
namespace Model
{

  /**
   * <p>Information about an account or service that has access to an Amazon
   * OpenSearch Service domain through the use of an interface VPC
   * endpoint.</p><p><h3>See Also:</h3>   <a
   * href="http://docs.aws.amazon.com/goto/WebAPI/es-2015-01-01/AuthorizedPrincipal">AWS
   * API Reference</a></p>
   */
  class AWS_ELASTICSEARCHSERVICE_API AuthorizedPrincipal
  {
  public:
    AuthorizedPrincipal();
    AuthorizedPrincipal(Aws::Utils::Json::JsonView jsonValue);
    AuthorizedPrincipal& operator=(Aws::Utils::Json::JsonView jsonValue);
    Aws::Utils::Json::JsonValue Jsonize() const;


    /**
     * <p>The type of principal.</p>
     */
    inline const PrincipalType& GetPrincipalType() const{ return m_principalType; }

    /**
     * <p>The type of principal.</p>
     */
    inline bool PrincipalTypeHasBeenSet() const { return m_principalTypeHasBeenSet; }

    /**
     * <p>The type of principal.</p>
     */
    inline void SetPrincipalType(const PrincipalType& value) { m_principalTypeHasBeenSet = true; m_principalType = value; }

    /**
     * <p>The type of principal.</p>
     */
    inline void SetPrincipalType(PrincipalType&& value) { m_principalTypeHasBeenSet = true; m_principalType = std::move(value); }

    /**
     * <p>The type of principal.</p>
     */
    inline AuthorizedPrincipal& WithPrincipalType(const PrincipalType& value) { SetPrincipalType(value); return *this;}

    /**
     * <p>The type of principal.</p>
     */
    inline AuthorizedPrincipal& WithPrincipalType(PrincipalType&& value) { SetPrincipalType(std::move(value)); return *this;}


    /**
     * <p>The IAM principal that is allowed access to the domain.</p>
     */
    inline const Aws::String& GetPrincipal() const{ return m_principal; }

    /**
     * <p>The IAM principal that is allowed access to the domain.</p>
     */
    inline bool PrincipalHasBeenSet() const { return m_principalHasBeenSet; }

    /**
     * <p>The IAM principal that is allowed access to the domain.</p>
     */
    inline void SetPrincipal(const Aws::String& value) { m_principalHasBeenSet = true; m_principal = value; }

    /**
     * <p>The IAM principal that is allowed access to the domain.</p>
     */
    inline void SetPrincipal(Aws::String&& value) { m_principalHasBeenSet = true; m_principal = std::move(value); }

    /**
     * <p>The IAM principal that is allowed access to the domain.</p>
     */
    inline void SetPrincipal(const char* value) { m_principalHasBeenSet = true; m_principal.assign(value); }

    /**
     * <p>The IAM principal that is allowed access to the domain.</p>
     */
    inline AuthorizedPrincipal& WithPrincipal(const Aws::String& value) { SetPrincipal(value); return *this;}

    /**
     * <p>The IAM principal that is allowed access to the domain.</p>
     */
    inline AuthorizedPrincipal& WithPrincipal(Aws::String&& value) { SetPrincipal(std::move(value)); return *this;}

    /**
     * <p>The IAM principal that is allowed access to the domain.</p>
     */
    inline AuthorizedPrincipal& WithPrincipal(const char* value) { SetPrincipal(value); return *this;}

  private:

    PrincipalType m_principalType;
    bool m_principalTypeHasBeenSet = false;

    Aws::String m_principal;
    bool m_principalHasBeenSet = false;
  };

} // namespace Model
} // namespace ElasticsearchService
} // namespace Aws
