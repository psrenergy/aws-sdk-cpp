﻿/**
 * Copyright Amazon.com, Inc. or its affiliates. All Rights Reserved.
 * SPDX-License-Identifier: Apache-2.0.
 */

#pragma once
#include <aws/cognito-identity/CognitoIdentity_EXPORTS.h>
#include <aws/cognito-identity/CognitoIdentityRequest.h>
#include <aws/core/utils/memory/stl/AWSString.h>
#include <aws/core/utils/memory/stl/AWSMap.h>
#include <aws/cognito-identity/model/RoleMapping.h>
#include <utility>

namespace Aws
{
namespace CognitoIdentity
{
namespace Model
{

  /**
   * <p>Input to the <code>SetIdentityPoolRoles</code> action.</p><p><h3>See
   * Also:</h3>   <a
   * href="http://docs.aws.amazon.com/goto/WebAPI/cognito-identity-2014-06-30/SetIdentityPoolRolesInput">AWS
   * API Reference</a></p>
   */
  class AWS_COGNITOIDENTITY_API SetIdentityPoolRolesRequest : public CognitoIdentityRequest
  {
  public:
    SetIdentityPoolRolesRequest();

    /**
     * Virtual Copy Constructor idiom;
     * Override in an inherited class to have overridden functions (such as GetRequestSpecificHeaders) be called by the SDK.
     * (If you are creating a child of this: don't forget to keep the original return type or (better) just use the 'override' keyword)
    */
    virtual Aws::UniquePtr<SetIdentityPoolRolesRequest> Clone() const
    {
      return Aws::MakeUnique<SetIdentityPoolRolesRequest>(GetServiceRequestName(), *this);
    }

    // Service request name is the Operation name which will send this request out,
    // each operation should has unique request name, so that we can get operation's name from this request.
    // Note: this is not true for response, multiple operations may have the same response name,
    // so we can not get operation's name from response.
    inline virtual const char* GetServiceRequestName() const override { return "SetIdentityPoolRoles"; }

    Aws::String SerializePayload() const override;

    Aws::Http::HeaderValueCollection GetRequestSpecificHeaders() const override;


    /**
     * <p>An identity pool ID in the format REGION:GUID.</p>
     */
    inline const Aws::String& GetIdentityPoolId() const{ return m_identityPoolId; }

    /**
     * <p>An identity pool ID in the format REGION:GUID.</p>
     */
    inline bool IdentityPoolIdHasBeenSet() const { return m_identityPoolIdHasBeenSet; }

    /**
     * <p>An identity pool ID in the format REGION:GUID.</p>
     */
    inline void SetIdentityPoolId(const Aws::String& value) { m_identityPoolIdHasBeenSet = true; m_identityPoolId = value; }

    /**
     * <p>An identity pool ID in the format REGION:GUID.</p>
     */
    inline void SetIdentityPoolId(Aws::String&& value) { m_identityPoolIdHasBeenSet = true; m_identityPoolId = std::move(value); }

    /**
     * <p>An identity pool ID in the format REGION:GUID.</p>
     */
    inline void SetIdentityPoolId(const char* value) { m_identityPoolIdHasBeenSet = true; m_identityPoolId.assign(value); }

    /**
     * <p>An identity pool ID in the format REGION:GUID.</p>
     */
    inline SetIdentityPoolRolesRequest& WithIdentityPoolId(const Aws::String& value) { SetIdentityPoolId(value); return *this;}

    /**
     * <p>An identity pool ID in the format REGION:GUID.</p>
     */
    inline SetIdentityPoolRolesRequest& WithIdentityPoolId(Aws::String&& value) { SetIdentityPoolId(std::move(value)); return *this;}

    /**
     * <p>An identity pool ID in the format REGION:GUID.</p>
     */
    inline SetIdentityPoolRolesRequest& WithIdentityPoolId(const char* value) { SetIdentityPoolId(value); return *this;}


    /**
     * <p>The map of roles associated with this pool. For a given role, the key will be
     * either "authenticated" or "unauthenticated" and the value will be the Role
     * ARN.</p>
     */
    inline const Aws::Map<Aws::String, Aws::String>& GetRoles() const{ return m_roles; }

    /**
     * <p>The map of roles associated with this pool. For a given role, the key will be
     * either "authenticated" or "unauthenticated" and the value will be the Role
     * ARN.</p>
     */
    inline bool RolesHasBeenSet() const { return m_rolesHasBeenSet; }

    /**
     * <p>The map of roles associated with this pool. For a given role, the key will be
     * either "authenticated" or "unauthenticated" and the value will be the Role
     * ARN.</p>
     */
    inline void SetRoles(const Aws::Map<Aws::String, Aws::String>& value) { m_rolesHasBeenSet = true; m_roles = value; }

    /**
     * <p>The map of roles associated with this pool. For a given role, the key will be
     * either "authenticated" or "unauthenticated" and the value will be the Role
     * ARN.</p>
     */
    inline void SetRoles(Aws::Map<Aws::String, Aws::String>&& value) { m_rolesHasBeenSet = true; m_roles = std::move(value); }

    /**
     * <p>The map of roles associated with this pool. For a given role, the key will be
     * either "authenticated" or "unauthenticated" and the value will be the Role
     * ARN.</p>
     */
    inline SetIdentityPoolRolesRequest& WithRoles(const Aws::Map<Aws::String, Aws::String>& value) { SetRoles(value); return *this;}

    /**
     * <p>The map of roles associated with this pool. For a given role, the key will be
     * either "authenticated" or "unauthenticated" and the value will be the Role
     * ARN.</p>
     */
    inline SetIdentityPoolRolesRequest& WithRoles(Aws::Map<Aws::String, Aws::String>&& value) { SetRoles(std::move(value)); return *this;}

    /**
     * <p>The map of roles associated with this pool. For a given role, the key will be
     * either "authenticated" or "unauthenticated" and the value will be the Role
     * ARN.</p>
     */
    inline SetIdentityPoolRolesRequest& AddRoles(const Aws::String& key, const Aws::String& value) { m_rolesHasBeenSet = true; m_roles.emplace(key, value); return *this; }

    /**
     * <p>The map of roles associated with this pool. For a given role, the key will be
     * either "authenticated" or "unauthenticated" and the value will be the Role
     * ARN.</p>
     */
    inline SetIdentityPoolRolesRequest& AddRoles(Aws::String&& key, const Aws::String& value) { m_rolesHasBeenSet = true; m_roles.emplace(std::move(key), value); return *this; }

    /**
     * <p>The map of roles associated with this pool. For a given role, the key will be
     * either "authenticated" or "unauthenticated" and the value will be the Role
     * ARN.</p>
     */
    inline SetIdentityPoolRolesRequest& AddRoles(const Aws::String& key, Aws::String&& value) { m_rolesHasBeenSet = true; m_roles.emplace(key, std::move(value)); return *this; }

    /**
     * <p>The map of roles associated with this pool. For a given role, the key will be
     * either "authenticated" or "unauthenticated" and the value will be the Role
     * ARN.</p>
     */
    inline SetIdentityPoolRolesRequest& AddRoles(Aws::String&& key, Aws::String&& value) { m_rolesHasBeenSet = true; m_roles.emplace(std::move(key), std::move(value)); return *this; }

    /**
     * <p>The map of roles associated with this pool. For a given role, the key will be
     * either "authenticated" or "unauthenticated" and the value will be the Role
     * ARN.</p>
     */
    inline SetIdentityPoolRolesRequest& AddRoles(const char* key, Aws::String&& value) { m_rolesHasBeenSet = true; m_roles.emplace(key, std::move(value)); return *this; }

    /**
     * <p>The map of roles associated with this pool. For a given role, the key will be
     * either "authenticated" or "unauthenticated" and the value will be the Role
     * ARN.</p>
     */
    inline SetIdentityPoolRolesRequest& AddRoles(Aws::String&& key, const char* value) { m_rolesHasBeenSet = true; m_roles.emplace(std::move(key), value); return *this; }

    /**
     * <p>The map of roles associated with this pool. For a given role, the key will be
     * either "authenticated" or "unauthenticated" and the value will be the Role
     * ARN.</p>
     */
    inline SetIdentityPoolRolesRequest& AddRoles(const char* key, const char* value) { m_rolesHasBeenSet = true; m_roles.emplace(key, value); return *this; }


    /**
     * <p>How users for a specific identity provider are to mapped to roles. This is a
     * string to <a>RoleMapping</a> object map. The string identifies the identity
     * provider, for example, "graph.facebook.com" or
     * "cognito-idp.us-east-1.amazonaws.com/us-east-1_abcdefghi:app_client_id".</p>
     * <p>Up to 25 rules can be specified per identity provider.</p>
     */
    inline const Aws::Map<Aws::String, RoleMapping>& GetRoleMappings() const{ return m_roleMappings; }

    /**
     * <p>How users for a specific identity provider are to mapped to roles. This is a
     * string to <a>RoleMapping</a> object map. The string identifies the identity
     * provider, for example, "graph.facebook.com" or
     * "cognito-idp.us-east-1.amazonaws.com/us-east-1_abcdefghi:app_client_id".</p>
     * <p>Up to 25 rules can be specified per identity provider.</p>
     */
    inline bool RoleMappingsHasBeenSet() const { return m_roleMappingsHasBeenSet; }

    /**
     * <p>How users for a specific identity provider are to mapped to roles. This is a
     * string to <a>RoleMapping</a> object map. The string identifies the identity
     * provider, for example, "graph.facebook.com" or
     * "cognito-idp.us-east-1.amazonaws.com/us-east-1_abcdefghi:app_client_id".</p>
     * <p>Up to 25 rules can be specified per identity provider.</p>
     */
    inline void SetRoleMappings(const Aws::Map<Aws::String, RoleMapping>& value) { m_roleMappingsHasBeenSet = true; m_roleMappings = value; }

    /**
     * <p>How users for a specific identity provider are to mapped to roles. This is a
     * string to <a>RoleMapping</a> object map. The string identifies the identity
     * provider, for example, "graph.facebook.com" or
     * "cognito-idp.us-east-1.amazonaws.com/us-east-1_abcdefghi:app_client_id".</p>
     * <p>Up to 25 rules can be specified per identity provider.</p>
     */
    inline void SetRoleMappings(Aws::Map<Aws::String, RoleMapping>&& value) { m_roleMappingsHasBeenSet = true; m_roleMappings = std::move(value); }

    /**
     * <p>How users for a specific identity provider are to mapped to roles. This is a
     * string to <a>RoleMapping</a> object map. The string identifies the identity
     * provider, for example, "graph.facebook.com" or
     * "cognito-idp.us-east-1.amazonaws.com/us-east-1_abcdefghi:app_client_id".</p>
     * <p>Up to 25 rules can be specified per identity provider.</p>
     */
    inline SetIdentityPoolRolesRequest& WithRoleMappings(const Aws::Map<Aws::String, RoleMapping>& value) { SetRoleMappings(value); return *this;}

    /**
     * <p>How users for a specific identity provider are to mapped to roles. This is a
     * string to <a>RoleMapping</a> object map. The string identifies the identity
     * provider, for example, "graph.facebook.com" or
     * "cognito-idp.us-east-1.amazonaws.com/us-east-1_abcdefghi:app_client_id".</p>
     * <p>Up to 25 rules can be specified per identity provider.</p>
     */
    inline SetIdentityPoolRolesRequest& WithRoleMappings(Aws::Map<Aws::String, RoleMapping>&& value) { SetRoleMappings(std::move(value)); return *this;}

    /**
     * <p>How users for a specific identity provider are to mapped to roles. This is a
     * string to <a>RoleMapping</a> object map. The string identifies the identity
     * provider, for example, "graph.facebook.com" or
     * "cognito-idp.us-east-1.amazonaws.com/us-east-1_abcdefghi:app_client_id".</p>
     * <p>Up to 25 rules can be specified per identity provider.</p>
     */
    inline SetIdentityPoolRolesRequest& AddRoleMappings(const Aws::String& key, const RoleMapping& value) { m_roleMappingsHasBeenSet = true; m_roleMappings.emplace(key, value); return *this; }

    /**
     * <p>How users for a specific identity provider are to mapped to roles. This is a
     * string to <a>RoleMapping</a> object map. The string identifies the identity
     * provider, for example, "graph.facebook.com" or
     * "cognito-idp.us-east-1.amazonaws.com/us-east-1_abcdefghi:app_client_id".</p>
     * <p>Up to 25 rules can be specified per identity provider.</p>
     */
    inline SetIdentityPoolRolesRequest& AddRoleMappings(Aws::String&& key, const RoleMapping& value) { m_roleMappingsHasBeenSet = true; m_roleMappings.emplace(std::move(key), value); return *this; }

    /**
     * <p>How users for a specific identity provider are to mapped to roles. This is a
     * string to <a>RoleMapping</a> object map. The string identifies the identity
     * provider, for example, "graph.facebook.com" or
     * "cognito-idp.us-east-1.amazonaws.com/us-east-1_abcdefghi:app_client_id".</p>
     * <p>Up to 25 rules can be specified per identity provider.</p>
     */
    inline SetIdentityPoolRolesRequest& AddRoleMappings(const Aws::String& key, RoleMapping&& value) { m_roleMappingsHasBeenSet = true; m_roleMappings.emplace(key, std::move(value)); return *this; }

    /**
     * <p>How users for a specific identity provider are to mapped to roles. This is a
     * string to <a>RoleMapping</a> object map. The string identifies the identity
     * provider, for example, "graph.facebook.com" or
     * "cognito-idp.us-east-1.amazonaws.com/us-east-1_abcdefghi:app_client_id".</p>
     * <p>Up to 25 rules can be specified per identity provider.</p>
     */
    inline SetIdentityPoolRolesRequest& AddRoleMappings(Aws::String&& key, RoleMapping&& value) { m_roleMappingsHasBeenSet = true; m_roleMappings.emplace(std::move(key), std::move(value)); return *this; }

    /**
     * <p>How users for a specific identity provider are to mapped to roles. This is a
     * string to <a>RoleMapping</a> object map. The string identifies the identity
     * provider, for example, "graph.facebook.com" or
     * "cognito-idp.us-east-1.amazonaws.com/us-east-1_abcdefghi:app_client_id".</p>
     * <p>Up to 25 rules can be specified per identity provider.</p>
     */
    inline SetIdentityPoolRolesRequest& AddRoleMappings(const char* key, RoleMapping&& value) { m_roleMappingsHasBeenSet = true; m_roleMappings.emplace(key, std::move(value)); return *this; }

    /**
     * <p>How users for a specific identity provider are to mapped to roles. This is a
     * string to <a>RoleMapping</a> object map. The string identifies the identity
     * provider, for example, "graph.facebook.com" or
     * "cognito-idp.us-east-1.amazonaws.com/us-east-1_abcdefghi:app_client_id".</p>
     * <p>Up to 25 rules can be specified per identity provider.</p>
     */
    inline SetIdentityPoolRolesRequest& AddRoleMappings(const char* key, const RoleMapping& value) { m_roleMappingsHasBeenSet = true; m_roleMappings.emplace(key, value); return *this; }

  private:

    Aws::String m_identityPoolId;
    bool m_identityPoolIdHasBeenSet = false;

    Aws::Map<Aws::String, Aws::String> m_roles;
    bool m_rolesHasBeenSet = false;

    Aws::Map<Aws::String, RoleMapping> m_roleMappings;
    bool m_roleMappingsHasBeenSet = false;
  };

} // namespace Model
} // namespace CognitoIdentity
} // namespace Aws
