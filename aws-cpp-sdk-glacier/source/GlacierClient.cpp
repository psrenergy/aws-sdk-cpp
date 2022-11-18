﻿/**
 * Copyright Amazon.com, Inc. or its affiliates. All Rights Reserved.
 * SPDX-License-Identifier: Apache-2.0.
 */

#include <aws/core/utils/Outcome.h>
#include <aws/core/auth/AWSAuthSigner.h>
#include <aws/core/client/CoreErrors.h>
#include <aws/core/client/RetryStrategy.h>
#include <aws/core/client/AWSAsyncOperationTemplate.h>
#include <aws/core/http/HttpClient.h>
#include <aws/core/http/HttpResponse.h>
#include <aws/core/http/HttpClientFactory.h>
#include <aws/core/auth/AWSCredentialsProviderChain.h>
#include <aws/core/utils/json/JsonSerializer.h>
#include <aws/core/utils/memory/stl/AWSStringStream.h>
#include <aws/core/utils/threading/Executor.h>
#include <aws/core/utils/DNS.h>
#include <aws/core/utils/logging/LogMacros.h>
#include <aws/core/utils/logging/ErrorMacros.h>

#include <aws/glacier/GlacierClient.h>
#include <aws/glacier/GlacierErrorMarshaller.h>
#include <aws/glacier/GlacierEndpointProvider.h>
#include <aws/glacier/model/AbortMultipartUploadRequest.h>
#include <aws/glacier/model/AbortVaultLockRequest.h>
#include <aws/glacier/model/AddTagsToVaultRequest.h>
#include <aws/glacier/model/CompleteMultipartUploadRequest.h>
#include <aws/glacier/model/CompleteVaultLockRequest.h>
#include <aws/glacier/model/CreateVaultRequest.h>
#include <aws/glacier/model/DeleteArchiveRequest.h>
#include <aws/glacier/model/DeleteVaultRequest.h>
#include <aws/glacier/model/DeleteVaultAccessPolicyRequest.h>
#include <aws/glacier/model/DeleteVaultNotificationsRequest.h>
#include <aws/glacier/model/DescribeJobRequest.h>
#include <aws/glacier/model/DescribeVaultRequest.h>
#include <aws/glacier/model/GetDataRetrievalPolicyRequest.h>
#include <aws/glacier/model/GetJobOutputRequest.h>
#include <aws/glacier/model/GetVaultAccessPolicyRequest.h>
#include <aws/glacier/model/GetVaultLockRequest.h>
#include <aws/glacier/model/GetVaultNotificationsRequest.h>
#include <aws/glacier/model/InitiateJobRequest.h>
#include <aws/glacier/model/InitiateMultipartUploadRequest.h>
#include <aws/glacier/model/InitiateVaultLockRequest.h>
#include <aws/glacier/model/ListJobsRequest.h>
#include <aws/glacier/model/ListMultipartUploadsRequest.h>
#include <aws/glacier/model/ListPartsRequest.h>
#include <aws/glacier/model/ListProvisionedCapacityRequest.h>
#include <aws/glacier/model/ListTagsForVaultRequest.h>
#include <aws/glacier/model/ListVaultsRequest.h>
#include <aws/glacier/model/PurchaseProvisionedCapacityRequest.h>
#include <aws/glacier/model/RemoveTagsFromVaultRequest.h>
#include <aws/glacier/model/SetDataRetrievalPolicyRequest.h>
#include <aws/glacier/model/SetVaultAccessPolicyRequest.h>
#include <aws/glacier/model/SetVaultNotificationsRequest.h>
#include <aws/glacier/model/UploadArchiveRequest.h>
#include <aws/glacier/model/UploadMultipartPartRequest.h>

using namespace Aws;
using namespace Aws::Auth;
using namespace Aws::Client;
using namespace Aws::Glacier;
using namespace Aws::Glacier::Model;
using namespace Aws::Http;
using namespace Aws::Utils::Json;
using ResolveEndpointOutcome = Aws::Endpoint::ResolveEndpointOutcome;

const char* GlacierClient::SERVICE_NAME = "glacier";
const char* GlacierClient::ALLOCATION_TAG = "GlacierClient";

GlacierClient::GlacierClient(const Glacier::GlacierClientConfiguration& clientConfiguration,
                             std::shared_ptr<GlacierEndpointProviderBase> endpointProvider) :
  BASECLASS(clientConfiguration,
            Aws::MakeShared<Aws::Auth::DefaultAuthSignerProvider>(ALLOCATION_TAG,
                                                                  Aws::MakeShared<DefaultAWSCredentialsProviderChain>(ALLOCATION_TAG),
                                                                  SERVICE_NAME,
                                                                  Aws::Region::ComputeSignerRegion(clientConfiguration.region)),
            Aws::MakeShared<GlacierErrorMarshaller>(ALLOCATION_TAG)),
  m_clientConfiguration(clientConfiguration),
  m_executor(clientConfiguration.executor),
  m_endpointProvider(std::move(endpointProvider))
{
  init(m_clientConfiguration);
}

GlacierClient::GlacierClient(const AWSCredentials& credentials,
                             std::shared_ptr<GlacierEndpointProviderBase> endpointProvider,
                             const Glacier::GlacierClientConfiguration& clientConfiguration) :
  BASECLASS(clientConfiguration,
            Aws::MakeShared<Aws::Auth::DefaultAuthSignerProvider>(ALLOCATION_TAG,
                                                                  Aws::MakeShared<SimpleAWSCredentialsProvider>(ALLOCATION_TAG, credentials),
                                                                  SERVICE_NAME,
                                                                  Aws::Region::ComputeSignerRegion(clientConfiguration.region)),
            Aws::MakeShared<GlacierErrorMarshaller>(ALLOCATION_TAG)),
    m_clientConfiguration(clientConfiguration),
    m_executor(clientConfiguration.executor),
    m_endpointProvider(std::move(endpointProvider))
{
  init(m_clientConfiguration);
}

GlacierClient::GlacierClient(const std::shared_ptr<AWSCredentialsProvider>& credentialsProvider,
                             std::shared_ptr<GlacierEndpointProviderBase> endpointProvider,
                             const Glacier::GlacierClientConfiguration& clientConfiguration) :
  BASECLASS(clientConfiguration,
            Aws::MakeShared<Aws::Auth::DefaultAuthSignerProvider>(ALLOCATION_TAG,
                                                                  credentialsProvider,
                                                                  SERVICE_NAME,
                                                                  Aws::Region::ComputeSignerRegion(clientConfiguration.region)),
            Aws::MakeShared<GlacierErrorMarshaller>(ALLOCATION_TAG)),
    m_clientConfiguration(clientConfiguration),
    m_executor(clientConfiguration.executor),
    m_endpointProvider(std::move(endpointProvider))
{
  init(m_clientConfiguration);
}

    /* Legacy constructors due deprecation */
  GlacierClient::GlacierClient(const Client::ClientConfiguration& clientConfiguration) :
  BASECLASS(clientConfiguration,
            Aws::MakeShared<Aws::Auth::DefaultAuthSignerProvider>(ALLOCATION_TAG,
                                                                  Aws::MakeShared<DefaultAWSCredentialsProviderChain>(ALLOCATION_TAG),
                                                                  SERVICE_NAME,
                                                                  Aws::Region::ComputeSignerRegion(clientConfiguration.region)),
            Aws::MakeShared<GlacierErrorMarshaller>(ALLOCATION_TAG)),
  m_clientConfiguration(clientConfiguration),
  m_executor(clientConfiguration.executor),
  m_endpointProvider(Aws::MakeShared<GlacierEndpointProvider>(ALLOCATION_TAG))
{
  init(m_clientConfiguration);
}

GlacierClient::GlacierClient(const AWSCredentials& credentials,
                             const Client::ClientConfiguration& clientConfiguration) :
  BASECLASS(clientConfiguration,
            Aws::MakeShared<Aws::Auth::DefaultAuthSignerProvider>(ALLOCATION_TAG,
                                                                  Aws::MakeShared<SimpleAWSCredentialsProvider>(ALLOCATION_TAG, credentials),
                                                                  SERVICE_NAME,
                                                                  Aws::Region::ComputeSignerRegion(clientConfiguration.region)),
            Aws::MakeShared<GlacierErrorMarshaller>(ALLOCATION_TAG)),
    m_clientConfiguration(clientConfiguration),
    m_executor(clientConfiguration.executor),
    m_endpointProvider(Aws::MakeShared<GlacierEndpointProvider>(ALLOCATION_TAG))
{
  init(m_clientConfiguration);
}

GlacierClient::GlacierClient(const std::shared_ptr<AWSCredentialsProvider>& credentialsProvider,
                             const Client::ClientConfiguration& clientConfiguration) :
  BASECLASS(clientConfiguration,
            Aws::MakeShared<Aws::Auth::DefaultAuthSignerProvider>(ALLOCATION_TAG,
                                                                  credentialsProvider,
                                                                  SERVICE_NAME,
                                                                  Aws::Region::ComputeSignerRegion(clientConfiguration.region)),
            Aws::MakeShared<GlacierErrorMarshaller>(ALLOCATION_TAG)),
    m_clientConfiguration(clientConfiguration),
    m_executor(clientConfiguration.executor),
    m_endpointProvider(Aws::MakeShared<GlacierEndpointProvider>(ALLOCATION_TAG))
{
  init(m_clientConfiguration);
}

    /* End of legacy constructors due deprecation */
GlacierClient::~GlacierClient()
{
}

std::shared_ptr<GlacierEndpointProviderBase>& GlacierClient::accessEndpointProvider()
{
  return m_endpointProvider;
}

void GlacierClient::init(const Glacier::GlacierClientConfiguration& config)
{
  AWSClient::SetServiceClientName("Glacier");
  AWS_CHECK_PTR(SERVICE_NAME, m_endpointProvider);
  m_endpointProvider->InitBuiltInParameters(config);
}

void GlacierClient::OverrideEndpoint(const Aws::String& endpoint)
{
  AWS_CHECK_PTR(SERVICE_NAME, m_endpointProvider);
  m_endpointProvider->OverrideEndpoint(endpoint);
}

AbortMultipartUploadOutcome GlacierClient::AbortMultipartUpload(const AbortMultipartUploadRequest& request) const
{
  AWS_OPERATION_CHECK_PTR(m_endpointProvider, AbortMultipartUpload, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE);
  if (!request.AccountIdHasBeenSet())
  {
    AWS_LOGSTREAM_ERROR("AbortMultipartUpload", "Required field: AccountId, is not set");
    return AbortMultipartUploadOutcome(Aws::Client::AWSError<GlacierErrors>(GlacierErrors::MISSING_PARAMETER, "MISSING_PARAMETER", "Missing required field [AccountId]", false));
  }
  if (!request.VaultNameHasBeenSet())
  {
    AWS_LOGSTREAM_ERROR("AbortMultipartUpload", "Required field: VaultName, is not set");
    return AbortMultipartUploadOutcome(Aws::Client::AWSError<GlacierErrors>(GlacierErrors::MISSING_PARAMETER, "MISSING_PARAMETER", "Missing required field [VaultName]", false));
  }
  if (!request.UploadIdHasBeenSet())
  {
    AWS_LOGSTREAM_ERROR("AbortMultipartUpload", "Required field: UploadId, is not set");
    return AbortMultipartUploadOutcome(Aws::Client::AWSError<GlacierErrors>(GlacierErrors::MISSING_PARAMETER, "MISSING_PARAMETER", "Missing required field [UploadId]", false));
  }
  if (request.GetAccountId().size() != 12 || request.GetAccountId().find_first_not_of("0123456789") != Aws::String::npos)
  {
    AWS_LOGSTREAM_ERROR("AbortMultipartUpload", "Required field: AccountId has invalid value");
    return AbortMultipartUploadOutcome(Aws::Client::AWSError<GlacierErrors>(GlacierErrors::INVALID_PARAMETER_VALUE, "INVALID_PARAMETER", "AccountId is invalid", false));
  }
  ResolveEndpointOutcome endpointResolutionOutcome = m_endpointProvider->ResolveEndpoint(request.GetEndpointContextParams());
  AWS_OPERATION_CHECK_SUCCESS(endpointResolutionOutcome, AbortMultipartUpload, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE, endpointResolutionOutcome.GetError().GetMessage());
  endpointResolutionOutcome.GetResult().AddPathSegment(request.GetAccountId());
  endpointResolutionOutcome.GetResult().AddPathSegments("/vaults/");
  endpointResolutionOutcome.GetResult().AddPathSegment(request.GetVaultName());
  endpointResolutionOutcome.GetResult().AddPathSegments("/multipart-uploads/");
  endpointResolutionOutcome.GetResult().AddPathSegment(request.GetUploadId());
  return AbortMultipartUploadOutcome(MakeRequest(request, endpointResolutionOutcome.GetResult(), Aws::Http::HttpMethod::HTTP_DELETE, Aws::Auth::SIGV4_SIGNER));
}

AbortMultipartUploadOutcomeCallable GlacierClient::AbortMultipartUploadCallable(const AbortMultipartUploadRequest& request) const
{
  AWS_MAKE_CALLABLE_OPERATION(AbortMultipartUpload, request, m_executor.get());
}

void GlacierClient::AbortMultipartUploadAsync(const AbortMultipartUploadRequest& request, const AbortMultipartUploadResponseReceivedHandler& handler, const std::shared_ptr<const Aws::Client::AsyncCallerContext>& context) const
{
  AWS_MAKE_ASYNC_OPERATION(AbortMultipartUpload, request, handler, context, m_executor.get());
}

AbortVaultLockOutcome GlacierClient::AbortVaultLock(const AbortVaultLockRequest& request) const
{
  AWS_OPERATION_CHECK_PTR(m_endpointProvider, AbortVaultLock, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE);
  if (!request.AccountIdHasBeenSet())
  {
    AWS_LOGSTREAM_ERROR("AbortVaultLock", "Required field: AccountId, is not set");
    return AbortVaultLockOutcome(Aws::Client::AWSError<GlacierErrors>(GlacierErrors::MISSING_PARAMETER, "MISSING_PARAMETER", "Missing required field [AccountId]", false));
  }
  if (!request.VaultNameHasBeenSet())
  {
    AWS_LOGSTREAM_ERROR("AbortVaultLock", "Required field: VaultName, is not set");
    return AbortVaultLockOutcome(Aws::Client::AWSError<GlacierErrors>(GlacierErrors::MISSING_PARAMETER, "MISSING_PARAMETER", "Missing required field [VaultName]", false));
  }
  if (request.GetAccountId().size() != 12 || request.GetAccountId().find_first_not_of("0123456789") != Aws::String::npos)
  {
    AWS_LOGSTREAM_ERROR("AbortVaultLock", "Required field: AccountId has invalid value");
    return AbortVaultLockOutcome(Aws::Client::AWSError<GlacierErrors>(GlacierErrors::INVALID_PARAMETER_VALUE, "INVALID_PARAMETER", "AccountId is invalid", false));
  }
  ResolveEndpointOutcome endpointResolutionOutcome = m_endpointProvider->ResolveEndpoint(request.GetEndpointContextParams());
  AWS_OPERATION_CHECK_SUCCESS(endpointResolutionOutcome, AbortVaultLock, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE, endpointResolutionOutcome.GetError().GetMessage());
  endpointResolutionOutcome.GetResult().AddPathSegment(request.GetAccountId());
  endpointResolutionOutcome.GetResult().AddPathSegments("/vaults/");
  endpointResolutionOutcome.GetResult().AddPathSegment(request.GetVaultName());
  endpointResolutionOutcome.GetResult().AddPathSegments("/lock-policy");
  return AbortVaultLockOutcome(MakeRequest(request, endpointResolutionOutcome.GetResult(), Aws::Http::HttpMethod::HTTP_DELETE, Aws::Auth::SIGV4_SIGNER));
}

AbortVaultLockOutcomeCallable GlacierClient::AbortVaultLockCallable(const AbortVaultLockRequest& request) const
{
  AWS_MAKE_CALLABLE_OPERATION(AbortVaultLock, request, m_executor.get());
}

void GlacierClient::AbortVaultLockAsync(const AbortVaultLockRequest& request, const AbortVaultLockResponseReceivedHandler& handler, const std::shared_ptr<const Aws::Client::AsyncCallerContext>& context) const
{
  AWS_MAKE_ASYNC_OPERATION(AbortVaultLock, request, handler, context, m_executor.get());
}

AddTagsToVaultOutcome GlacierClient::AddTagsToVault(const AddTagsToVaultRequest& request) const
{
  AWS_OPERATION_CHECK_PTR(m_endpointProvider, AddTagsToVault, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE);
  if (!request.AccountIdHasBeenSet())
  {
    AWS_LOGSTREAM_ERROR("AddTagsToVault", "Required field: AccountId, is not set");
    return AddTagsToVaultOutcome(Aws::Client::AWSError<GlacierErrors>(GlacierErrors::MISSING_PARAMETER, "MISSING_PARAMETER", "Missing required field [AccountId]", false));
  }
  if (!request.VaultNameHasBeenSet())
  {
    AWS_LOGSTREAM_ERROR("AddTagsToVault", "Required field: VaultName, is not set");
    return AddTagsToVaultOutcome(Aws::Client::AWSError<GlacierErrors>(GlacierErrors::MISSING_PARAMETER, "MISSING_PARAMETER", "Missing required field [VaultName]", false));
  }
  if (request.GetAccountId().size() != 12 || request.GetAccountId().find_first_not_of("0123456789") != Aws::String::npos)
  {
    AWS_LOGSTREAM_ERROR("AddTagsToVault", "Required field: AccountId has invalid value");
    return AddTagsToVaultOutcome(Aws::Client::AWSError<GlacierErrors>(GlacierErrors::INVALID_PARAMETER_VALUE, "INVALID_PARAMETER", "AccountId is invalid", false));
  }
  ResolveEndpointOutcome endpointResolutionOutcome = m_endpointProvider->ResolveEndpoint(request.GetEndpointContextParams());
  AWS_OPERATION_CHECK_SUCCESS(endpointResolutionOutcome, AddTagsToVault, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE, endpointResolutionOutcome.GetError().GetMessage());
  Aws::StringStream ss;
  endpointResolutionOutcome.GetResult().AddPathSegment(request.GetAccountId());
  endpointResolutionOutcome.GetResult().AddPathSegments("/vaults/");
  endpointResolutionOutcome.GetResult().AddPathSegment(request.GetVaultName());
  endpointResolutionOutcome.GetResult().AddPathSegments("/tags");
  ss.str("?operation=add");
  endpointResolutionOutcome.GetResult().SetQueryString(ss.str());
  return AddTagsToVaultOutcome(MakeRequest(request, endpointResolutionOutcome.GetResult(), Aws::Http::HttpMethod::HTTP_POST, Aws::Auth::SIGV4_SIGNER));
}

AddTagsToVaultOutcomeCallable GlacierClient::AddTagsToVaultCallable(const AddTagsToVaultRequest& request) const
{
  AWS_MAKE_CALLABLE_OPERATION(AddTagsToVault, request, m_executor.get());
}

void GlacierClient::AddTagsToVaultAsync(const AddTagsToVaultRequest& request, const AddTagsToVaultResponseReceivedHandler& handler, const std::shared_ptr<const Aws::Client::AsyncCallerContext>& context) const
{
  AWS_MAKE_ASYNC_OPERATION(AddTagsToVault, request, handler, context, m_executor.get());
}

CompleteMultipartUploadOutcome GlacierClient::CompleteMultipartUpload(const CompleteMultipartUploadRequest& request) const
{
  AWS_OPERATION_CHECK_PTR(m_endpointProvider, CompleteMultipartUpload, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE);
  if (!request.AccountIdHasBeenSet())
  {
    AWS_LOGSTREAM_ERROR("CompleteMultipartUpload", "Required field: AccountId, is not set");
    return CompleteMultipartUploadOutcome(Aws::Client::AWSError<GlacierErrors>(GlacierErrors::MISSING_PARAMETER, "MISSING_PARAMETER", "Missing required field [AccountId]", false));
  }
  if (!request.VaultNameHasBeenSet())
  {
    AWS_LOGSTREAM_ERROR("CompleteMultipartUpload", "Required field: VaultName, is not set");
    return CompleteMultipartUploadOutcome(Aws::Client::AWSError<GlacierErrors>(GlacierErrors::MISSING_PARAMETER, "MISSING_PARAMETER", "Missing required field [VaultName]", false));
  }
  if (!request.UploadIdHasBeenSet())
  {
    AWS_LOGSTREAM_ERROR("CompleteMultipartUpload", "Required field: UploadId, is not set");
    return CompleteMultipartUploadOutcome(Aws::Client::AWSError<GlacierErrors>(GlacierErrors::MISSING_PARAMETER, "MISSING_PARAMETER", "Missing required field [UploadId]", false));
  }
  if (request.GetAccountId().size() != 12 || request.GetAccountId().find_first_not_of("0123456789") != Aws::String::npos)
  {
    AWS_LOGSTREAM_ERROR("CompleteMultipartUpload", "Required field: AccountId has invalid value");
    return CompleteMultipartUploadOutcome(Aws::Client::AWSError<GlacierErrors>(GlacierErrors::INVALID_PARAMETER_VALUE, "INVALID_PARAMETER", "AccountId is invalid", false));
  }
  ResolveEndpointOutcome endpointResolutionOutcome = m_endpointProvider->ResolveEndpoint(request.GetEndpointContextParams());
  AWS_OPERATION_CHECK_SUCCESS(endpointResolutionOutcome, CompleteMultipartUpload, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE, endpointResolutionOutcome.GetError().GetMessage());
  endpointResolutionOutcome.GetResult().AddPathSegment(request.GetAccountId());
  endpointResolutionOutcome.GetResult().AddPathSegments("/vaults/");
  endpointResolutionOutcome.GetResult().AddPathSegment(request.GetVaultName());
  endpointResolutionOutcome.GetResult().AddPathSegments("/multipart-uploads/");
  endpointResolutionOutcome.GetResult().AddPathSegment(request.GetUploadId());
  return CompleteMultipartUploadOutcome(MakeRequest(request, endpointResolutionOutcome.GetResult(), Aws::Http::HttpMethod::HTTP_POST, Aws::Auth::SIGV4_SIGNER));
}

CompleteMultipartUploadOutcomeCallable GlacierClient::CompleteMultipartUploadCallable(const CompleteMultipartUploadRequest& request) const
{
  AWS_MAKE_CALLABLE_OPERATION(CompleteMultipartUpload, request, m_executor.get());
}

void GlacierClient::CompleteMultipartUploadAsync(const CompleteMultipartUploadRequest& request, const CompleteMultipartUploadResponseReceivedHandler& handler, const std::shared_ptr<const Aws::Client::AsyncCallerContext>& context) const
{
  AWS_MAKE_ASYNC_OPERATION(CompleteMultipartUpload, request, handler, context, m_executor.get());
}

CompleteVaultLockOutcome GlacierClient::CompleteVaultLock(const CompleteVaultLockRequest& request) const
{
  AWS_OPERATION_CHECK_PTR(m_endpointProvider, CompleteVaultLock, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE);
  if (!request.AccountIdHasBeenSet())
  {
    AWS_LOGSTREAM_ERROR("CompleteVaultLock", "Required field: AccountId, is not set");
    return CompleteVaultLockOutcome(Aws::Client::AWSError<GlacierErrors>(GlacierErrors::MISSING_PARAMETER, "MISSING_PARAMETER", "Missing required field [AccountId]", false));
  }
  if (!request.VaultNameHasBeenSet())
  {
    AWS_LOGSTREAM_ERROR("CompleteVaultLock", "Required field: VaultName, is not set");
    return CompleteVaultLockOutcome(Aws::Client::AWSError<GlacierErrors>(GlacierErrors::MISSING_PARAMETER, "MISSING_PARAMETER", "Missing required field [VaultName]", false));
  }
  if (!request.LockIdHasBeenSet())
  {
    AWS_LOGSTREAM_ERROR("CompleteVaultLock", "Required field: LockId, is not set");
    return CompleteVaultLockOutcome(Aws::Client::AWSError<GlacierErrors>(GlacierErrors::MISSING_PARAMETER, "MISSING_PARAMETER", "Missing required field [LockId]", false));
  }
  if (request.GetAccountId().size() != 12 || request.GetAccountId().find_first_not_of("0123456789") != Aws::String::npos)
  {
    AWS_LOGSTREAM_ERROR("CompleteVaultLock", "Required field: AccountId has invalid value");
    return CompleteVaultLockOutcome(Aws::Client::AWSError<GlacierErrors>(GlacierErrors::INVALID_PARAMETER_VALUE, "INVALID_PARAMETER", "AccountId is invalid", false));
  }
  ResolveEndpointOutcome endpointResolutionOutcome = m_endpointProvider->ResolveEndpoint(request.GetEndpointContextParams());
  AWS_OPERATION_CHECK_SUCCESS(endpointResolutionOutcome, CompleteVaultLock, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE, endpointResolutionOutcome.GetError().GetMessage());
  endpointResolutionOutcome.GetResult().AddPathSegment(request.GetAccountId());
  endpointResolutionOutcome.GetResult().AddPathSegments("/vaults/");
  endpointResolutionOutcome.GetResult().AddPathSegment(request.GetVaultName());
  endpointResolutionOutcome.GetResult().AddPathSegments("/lock-policy/");
  endpointResolutionOutcome.GetResult().AddPathSegment(request.GetLockId());
  return CompleteVaultLockOutcome(MakeRequest(request, endpointResolutionOutcome.GetResult(), Aws::Http::HttpMethod::HTTP_POST, Aws::Auth::SIGV4_SIGNER));
}

CompleteVaultLockOutcomeCallable GlacierClient::CompleteVaultLockCallable(const CompleteVaultLockRequest& request) const
{
  AWS_MAKE_CALLABLE_OPERATION(CompleteVaultLock, request, m_executor.get());
}

void GlacierClient::CompleteVaultLockAsync(const CompleteVaultLockRequest& request, const CompleteVaultLockResponseReceivedHandler& handler, const std::shared_ptr<const Aws::Client::AsyncCallerContext>& context) const
{
  AWS_MAKE_ASYNC_OPERATION(CompleteVaultLock, request, handler, context, m_executor.get());
}

CreateVaultOutcome GlacierClient::CreateVault(const CreateVaultRequest& request) const
{
  AWS_OPERATION_CHECK_PTR(m_endpointProvider, CreateVault, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE);
  if (!request.AccountIdHasBeenSet())
  {
    AWS_LOGSTREAM_ERROR("CreateVault", "Required field: AccountId, is not set");
    return CreateVaultOutcome(Aws::Client::AWSError<GlacierErrors>(GlacierErrors::MISSING_PARAMETER, "MISSING_PARAMETER", "Missing required field [AccountId]", false));
  }
  if (!request.VaultNameHasBeenSet())
  {
    AWS_LOGSTREAM_ERROR("CreateVault", "Required field: VaultName, is not set");
    return CreateVaultOutcome(Aws::Client::AWSError<GlacierErrors>(GlacierErrors::MISSING_PARAMETER, "MISSING_PARAMETER", "Missing required field [VaultName]", false));
  }
  if (request.GetAccountId().size() != 12 || request.GetAccountId().find_first_not_of("0123456789") != Aws::String::npos)
  {
    AWS_LOGSTREAM_ERROR("CreateVault", "Required field: AccountId has invalid value");
    return CreateVaultOutcome(Aws::Client::AWSError<GlacierErrors>(GlacierErrors::INVALID_PARAMETER_VALUE, "INVALID_PARAMETER", "AccountId is invalid", false));
  }
  ResolveEndpointOutcome endpointResolutionOutcome = m_endpointProvider->ResolveEndpoint(request.GetEndpointContextParams());
  AWS_OPERATION_CHECK_SUCCESS(endpointResolutionOutcome, CreateVault, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE, endpointResolutionOutcome.GetError().GetMessage());
  endpointResolutionOutcome.GetResult().AddPathSegment(request.GetAccountId());
  endpointResolutionOutcome.GetResult().AddPathSegments("/vaults/");
  endpointResolutionOutcome.GetResult().AddPathSegment(request.GetVaultName());
  return CreateVaultOutcome(MakeRequest(request, endpointResolutionOutcome.GetResult(), Aws::Http::HttpMethod::HTTP_PUT, Aws::Auth::SIGV4_SIGNER));
}

CreateVaultOutcomeCallable GlacierClient::CreateVaultCallable(const CreateVaultRequest& request) const
{
  AWS_MAKE_CALLABLE_OPERATION(CreateVault, request, m_executor.get());
}

void GlacierClient::CreateVaultAsync(const CreateVaultRequest& request, const CreateVaultResponseReceivedHandler& handler, const std::shared_ptr<const Aws::Client::AsyncCallerContext>& context) const
{
  AWS_MAKE_ASYNC_OPERATION(CreateVault, request, handler, context, m_executor.get());
}

DeleteArchiveOutcome GlacierClient::DeleteArchive(const DeleteArchiveRequest& request) const
{
  AWS_OPERATION_CHECK_PTR(m_endpointProvider, DeleteArchive, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE);
  if (!request.AccountIdHasBeenSet())
  {
    AWS_LOGSTREAM_ERROR("DeleteArchive", "Required field: AccountId, is not set");
    return DeleteArchiveOutcome(Aws::Client::AWSError<GlacierErrors>(GlacierErrors::MISSING_PARAMETER, "MISSING_PARAMETER", "Missing required field [AccountId]", false));
  }
  if (!request.VaultNameHasBeenSet())
  {
    AWS_LOGSTREAM_ERROR("DeleteArchive", "Required field: VaultName, is not set");
    return DeleteArchiveOutcome(Aws::Client::AWSError<GlacierErrors>(GlacierErrors::MISSING_PARAMETER, "MISSING_PARAMETER", "Missing required field [VaultName]", false));
  }
  if (!request.ArchiveIdHasBeenSet())
  {
    AWS_LOGSTREAM_ERROR("DeleteArchive", "Required field: ArchiveId, is not set");
    return DeleteArchiveOutcome(Aws::Client::AWSError<GlacierErrors>(GlacierErrors::MISSING_PARAMETER, "MISSING_PARAMETER", "Missing required field [ArchiveId]", false));
  }
  if (request.GetAccountId().size() != 12 || request.GetAccountId().find_first_not_of("0123456789") != Aws::String::npos)
  {
    AWS_LOGSTREAM_ERROR("DeleteArchive", "Required field: AccountId has invalid value");
    return DeleteArchiveOutcome(Aws::Client::AWSError<GlacierErrors>(GlacierErrors::INVALID_PARAMETER_VALUE, "INVALID_PARAMETER", "AccountId is invalid", false));
  }
  ResolveEndpointOutcome endpointResolutionOutcome = m_endpointProvider->ResolveEndpoint(request.GetEndpointContextParams());
  AWS_OPERATION_CHECK_SUCCESS(endpointResolutionOutcome, DeleteArchive, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE, endpointResolutionOutcome.GetError().GetMessage());
  endpointResolutionOutcome.GetResult().AddPathSegment(request.GetAccountId());
  endpointResolutionOutcome.GetResult().AddPathSegments("/vaults/");
  endpointResolutionOutcome.GetResult().AddPathSegment(request.GetVaultName());
  endpointResolutionOutcome.GetResult().AddPathSegments("/archives/");
  endpointResolutionOutcome.GetResult().AddPathSegment(request.GetArchiveId());
  return DeleteArchiveOutcome(MakeRequest(request, endpointResolutionOutcome.GetResult(), Aws::Http::HttpMethod::HTTP_DELETE, Aws::Auth::SIGV4_SIGNER));
}

DeleteArchiveOutcomeCallable GlacierClient::DeleteArchiveCallable(const DeleteArchiveRequest& request) const
{
  AWS_MAKE_CALLABLE_OPERATION(DeleteArchive, request, m_executor.get());
}

void GlacierClient::DeleteArchiveAsync(const DeleteArchiveRequest& request, const DeleteArchiveResponseReceivedHandler& handler, const std::shared_ptr<const Aws::Client::AsyncCallerContext>& context) const
{
  AWS_MAKE_ASYNC_OPERATION(DeleteArchive, request, handler, context, m_executor.get());
}

DeleteVaultOutcome GlacierClient::DeleteVault(const DeleteVaultRequest& request) const
{
  AWS_OPERATION_CHECK_PTR(m_endpointProvider, DeleteVault, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE);
  if (!request.AccountIdHasBeenSet())
  {
    AWS_LOGSTREAM_ERROR("DeleteVault", "Required field: AccountId, is not set");
    return DeleteVaultOutcome(Aws::Client::AWSError<GlacierErrors>(GlacierErrors::MISSING_PARAMETER, "MISSING_PARAMETER", "Missing required field [AccountId]", false));
  }
  if (!request.VaultNameHasBeenSet())
  {
    AWS_LOGSTREAM_ERROR("DeleteVault", "Required field: VaultName, is not set");
    return DeleteVaultOutcome(Aws::Client::AWSError<GlacierErrors>(GlacierErrors::MISSING_PARAMETER, "MISSING_PARAMETER", "Missing required field [VaultName]", false));
  }
  if (request.GetAccountId().size() != 12 || request.GetAccountId().find_first_not_of("0123456789") != Aws::String::npos)
  {
    AWS_LOGSTREAM_ERROR("DeleteVault", "Required field: AccountId has invalid value");
    return DeleteVaultOutcome(Aws::Client::AWSError<GlacierErrors>(GlacierErrors::INVALID_PARAMETER_VALUE, "INVALID_PARAMETER", "AccountId is invalid", false));
  }
  ResolveEndpointOutcome endpointResolutionOutcome = m_endpointProvider->ResolveEndpoint(request.GetEndpointContextParams());
  AWS_OPERATION_CHECK_SUCCESS(endpointResolutionOutcome, DeleteVault, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE, endpointResolutionOutcome.GetError().GetMessage());
  endpointResolutionOutcome.GetResult().AddPathSegment(request.GetAccountId());
  endpointResolutionOutcome.GetResult().AddPathSegments("/vaults/");
  endpointResolutionOutcome.GetResult().AddPathSegment(request.GetVaultName());
  return DeleteVaultOutcome(MakeRequest(request, endpointResolutionOutcome.GetResult(), Aws::Http::HttpMethod::HTTP_DELETE, Aws::Auth::SIGV4_SIGNER));
}

DeleteVaultOutcomeCallable GlacierClient::DeleteVaultCallable(const DeleteVaultRequest& request) const
{
  AWS_MAKE_CALLABLE_OPERATION(DeleteVault, request, m_executor.get());
}

void GlacierClient::DeleteVaultAsync(const DeleteVaultRequest& request, const DeleteVaultResponseReceivedHandler& handler, const std::shared_ptr<const Aws::Client::AsyncCallerContext>& context) const
{
  AWS_MAKE_ASYNC_OPERATION(DeleteVault, request, handler, context, m_executor.get());
}

DeleteVaultAccessPolicyOutcome GlacierClient::DeleteVaultAccessPolicy(const DeleteVaultAccessPolicyRequest& request) const
{
  AWS_OPERATION_CHECK_PTR(m_endpointProvider, DeleteVaultAccessPolicy, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE);
  if (!request.AccountIdHasBeenSet())
  {
    AWS_LOGSTREAM_ERROR("DeleteVaultAccessPolicy", "Required field: AccountId, is not set");
    return DeleteVaultAccessPolicyOutcome(Aws::Client::AWSError<GlacierErrors>(GlacierErrors::MISSING_PARAMETER, "MISSING_PARAMETER", "Missing required field [AccountId]", false));
  }
  if (!request.VaultNameHasBeenSet())
  {
    AWS_LOGSTREAM_ERROR("DeleteVaultAccessPolicy", "Required field: VaultName, is not set");
    return DeleteVaultAccessPolicyOutcome(Aws::Client::AWSError<GlacierErrors>(GlacierErrors::MISSING_PARAMETER, "MISSING_PARAMETER", "Missing required field [VaultName]", false));
  }
  if (request.GetAccountId().size() != 12 || request.GetAccountId().find_first_not_of("0123456789") != Aws::String::npos)
  {
    AWS_LOGSTREAM_ERROR("DeleteVaultAccessPolicy", "Required field: AccountId has invalid value");
    return DeleteVaultAccessPolicyOutcome(Aws::Client::AWSError<GlacierErrors>(GlacierErrors::INVALID_PARAMETER_VALUE, "INVALID_PARAMETER", "AccountId is invalid", false));
  }
  ResolveEndpointOutcome endpointResolutionOutcome = m_endpointProvider->ResolveEndpoint(request.GetEndpointContextParams());
  AWS_OPERATION_CHECK_SUCCESS(endpointResolutionOutcome, DeleteVaultAccessPolicy, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE, endpointResolutionOutcome.GetError().GetMessage());
  endpointResolutionOutcome.GetResult().AddPathSegment(request.GetAccountId());
  endpointResolutionOutcome.GetResult().AddPathSegments("/vaults/");
  endpointResolutionOutcome.GetResult().AddPathSegment(request.GetVaultName());
  endpointResolutionOutcome.GetResult().AddPathSegments("/access-policy");
  return DeleteVaultAccessPolicyOutcome(MakeRequest(request, endpointResolutionOutcome.GetResult(), Aws::Http::HttpMethod::HTTP_DELETE, Aws::Auth::SIGV4_SIGNER));
}

DeleteVaultAccessPolicyOutcomeCallable GlacierClient::DeleteVaultAccessPolicyCallable(const DeleteVaultAccessPolicyRequest& request) const
{
  AWS_MAKE_CALLABLE_OPERATION(DeleteVaultAccessPolicy, request, m_executor.get());
}

void GlacierClient::DeleteVaultAccessPolicyAsync(const DeleteVaultAccessPolicyRequest& request, const DeleteVaultAccessPolicyResponseReceivedHandler& handler, const std::shared_ptr<const Aws::Client::AsyncCallerContext>& context) const
{
  AWS_MAKE_ASYNC_OPERATION(DeleteVaultAccessPolicy, request, handler, context, m_executor.get());
}

DeleteVaultNotificationsOutcome GlacierClient::DeleteVaultNotifications(const DeleteVaultNotificationsRequest& request) const
{
  AWS_OPERATION_CHECK_PTR(m_endpointProvider, DeleteVaultNotifications, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE);
  if (!request.AccountIdHasBeenSet())
  {
    AWS_LOGSTREAM_ERROR("DeleteVaultNotifications", "Required field: AccountId, is not set");
    return DeleteVaultNotificationsOutcome(Aws::Client::AWSError<GlacierErrors>(GlacierErrors::MISSING_PARAMETER, "MISSING_PARAMETER", "Missing required field [AccountId]", false));
  }
  if (!request.VaultNameHasBeenSet())
  {
    AWS_LOGSTREAM_ERROR("DeleteVaultNotifications", "Required field: VaultName, is not set");
    return DeleteVaultNotificationsOutcome(Aws::Client::AWSError<GlacierErrors>(GlacierErrors::MISSING_PARAMETER, "MISSING_PARAMETER", "Missing required field [VaultName]", false));
  }
  if (request.GetAccountId().size() != 12 || request.GetAccountId().find_first_not_of("0123456789") != Aws::String::npos)
  {
    AWS_LOGSTREAM_ERROR("DeleteVaultNotifications", "Required field: AccountId has invalid value");
    return DeleteVaultNotificationsOutcome(Aws::Client::AWSError<GlacierErrors>(GlacierErrors::INVALID_PARAMETER_VALUE, "INVALID_PARAMETER", "AccountId is invalid", false));
  }
  ResolveEndpointOutcome endpointResolutionOutcome = m_endpointProvider->ResolveEndpoint(request.GetEndpointContextParams());
  AWS_OPERATION_CHECK_SUCCESS(endpointResolutionOutcome, DeleteVaultNotifications, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE, endpointResolutionOutcome.GetError().GetMessage());
  endpointResolutionOutcome.GetResult().AddPathSegment(request.GetAccountId());
  endpointResolutionOutcome.GetResult().AddPathSegments("/vaults/");
  endpointResolutionOutcome.GetResult().AddPathSegment(request.GetVaultName());
  endpointResolutionOutcome.GetResult().AddPathSegments("/notification-configuration");
  return DeleteVaultNotificationsOutcome(MakeRequest(request, endpointResolutionOutcome.GetResult(), Aws::Http::HttpMethod::HTTP_DELETE, Aws::Auth::SIGV4_SIGNER));
}

DeleteVaultNotificationsOutcomeCallable GlacierClient::DeleteVaultNotificationsCallable(const DeleteVaultNotificationsRequest& request) const
{
  AWS_MAKE_CALLABLE_OPERATION(DeleteVaultNotifications, request, m_executor.get());
}

void GlacierClient::DeleteVaultNotificationsAsync(const DeleteVaultNotificationsRequest& request, const DeleteVaultNotificationsResponseReceivedHandler& handler, const std::shared_ptr<const Aws::Client::AsyncCallerContext>& context) const
{
  AWS_MAKE_ASYNC_OPERATION(DeleteVaultNotifications, request, handler, context, m_executor.get());
}

DescribeJobOutcome GlacierClient::DescribeJob(const DescribeJobRequest& request) const
{
  AWS_OPERATION_CHECK_PTR(m_endpointProvider, DescribeJob, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE);
  if (!request.AccountIdHasBeenSet())
  {
    AWS_LOGSTREAM_ERROR("DescribeJob", "Required field: AccountId, is not set");
    return DescribeJobOutcome(Aws::Client::AWSError<GlacierErrors>(GlacierErrors::MISSING_PARAMETER, "MISSING_PARAMETER", "Missing required field [AccountId]", false));
  }
  if (!request.VaultNameHasBeenSet())
  {
    AWS_LOGSTREAM_ERROR("DescribeJob", "Required field: VaultName, is not set");
    return DescribeJobOutcome(Aws::Client::AWSError<GlacierErrors>(GlacierErrors::MISSING_PARAMETER, "MISSING_PARAMETER", "Missing required field [VaultName]", false));
  }
  if (!request.JobIdHasBeenSet())
  {
    AWS_LOGSTREAM_ERROR("DescribeJob", "Required field: JobId, is not set");
    return DescribeJobOutcome(Aws::Client::AWSError<GlacierErrors>(GlacierErrors::MISSING_PARAMETER, "MISSING_PARAMETER", "Missing required field [JobId]", false));
  }
  if (request.GetAccountId().size() != 12 || request.GetAccountId().find_first_not_of("0123456789") != Aws::String::npos)
  {
    AWS_LOGSTREAM_ERROR("DescribeJob", "Required field: AccountId has invalid value");
    return DescribeJobOutcome(Aws::Client::AWSError<GlacierErrors>(GlacierErrors::INVALID_PARAMETER_VALUE, "INVALID_PARAMETER", "AccountId is invalid", false));
  }
  ResolveEndpointOutcome endpointResolutionOutcome = m_endpointProvider->ResolveEndpoint(request.GetEndpointContextParams());
  AWS_OPERATION_CHECK_SUCCESS(endpointResolutionOutcome, DescribeJob, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE, endpointResolutionOutcome.GetError().GetMessage());
  endpointResolutionOutcome.GetResult().AddPathSegment(request.GetAccountId());
  endpointResolutionOutcome.GetResult().AddPathSegments("/vaults/");
  endpointResolutionOutcome.GetResult().AddPathSegment(request.GetVaultName());
  endpointResolutionOutcome.GetResult().AddPathSegments("/jobs/");
  endpointResolutionOutcome.GetResult().AddPathSegment(request.GetJobId());
  return DescribeJobOutcome(MakeRequest(request, endpointResolutionOutcome.GetResult(), Aws::Http::HttpMethod::HTTP_GET, Aws::Auth::SIGV4_SIGNER));
}

DescribeJobOutcomeCallable GlacierClient::DescribeJobCallable(const DescribeJobRequest& request) const
{
  AWS_MAKE_CALLABLE_OPERATION(DescribeJob, request, m_executor.get());
}

void GlacierClient::DescribeJobAsync(const DescribeJobRequest& request, const DescribeJobResponseReceivedHandler& handler, const std::shared_ptr<const Aws::Client::AsyncCallerContext>& context) const
{
  AWS_MAKE_ASYNC_OPERATION(DescribeJob, request, handler, context, m_executor.get());
}

DescribeVaultOutcome GlacierClient::DescribeVault(const DescribeVaultRequest& request) const
{
  AWS_OPERATION_CHECK_PTR(m_endpointProvider, DescribeVault, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE);
  if (!request.AccountIdHasBeenSet())
  {
    AWS_LOGSTREAM_ERROR("DescribeVault", "Required field: AccountId, is not set");
    return DescribeVaultOutcome(Aws::Client::AWSError<GlacierErrors>(GlacierErrors::MISSING_PARAMETER, "MISSING_PARAMETER", "Missing required field [AccountId]", false));
  }
  if (!request.VaultNameHasBeenSet())
  {
    AWS_LOGSTREAM_ERROR("DescribeVault", "Required field: VaultName, is not set");
    return DescribeVaultOutcome(Aws::Client::AWSError<GlacierErrors>(GlacierErrors::MISSING_PARAMETER, "MISSING_PARAMETER", "Missing required field [VaultName]", false));
  }
  if (request.GetAccountId().size() != 12 || request.GetAccountId().find_first_not_of("0123456789") != Aws::String::npos)
  {
    AWS_LOGSTREAM_ERROR("DescribeVault", "Required field: AccountId has invalid value");
    return DescribeVaultOutcome(Aws::Client::AWSError<GlacierErrors>(GlacierErrors::INVALID_PARAMETER_VALUE, "INVALID_PARAMETER", "AccountId is invalid", false));
  }
  ResolveEndpointOutcome endpointResolutionOutcome = m_endpointProvider->ResolveEndpoint(request.GetEndpointContextParams());
  AWS_OPERATION_CHECK_SUCCESS(endpointResolutionOutcome, DescribeVault, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE, endpointResolutionOutcome.GetError().GetMessage());
  endpointResolutionOutcome.GetResult().AddPathSegment(request.GetAccountId());
  endpointResolutionOutcome.GetResult().AddPathSegments("/vaults/");
  endpointResolutionOutcome.GetResult().AddPathSegment(request.GetVaultName());
  return DescribeVaultOutcome(MakeRequest(request, endpointResolutionOutcome.GetResult(), Aws::Http::HttpMethod::HTTP_GET, Aws::Auth::SIGV4_SIGNER));
}

DescribeVaultOutcomeCallable GlacierClient::DescribeVaultCallable(const DescribeVaultRequest& request) const
{
  AWS_MAKE_CALLABLE_OPERATION(DescribeVault, request, m_executor.get());
}

void GlacierClient::DescribeVaultAsync(const DescribeVaultRequest& request, const DescribeVaultResponseReceivedHandler& handler, const std::shared_ptr<const Aws::Client::AsyncCallerContext>& context) const
{
  AWS_MAKE_ASYNC_OPERATION(DescribeVault, request, handler, context, m_executor.get());
}

GetDataRetrievalPolicyOutcome GlacierClient::GetDataRetrievalPolicy(const GetDataRetrievalPolicyRequest& request) const
{
  AWS_OPERATION_CHECK_PTR(m_endpointProvider, GetDataRetrievalPolicy, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE);
  if (!request.AccountIdHasBeenSet())
  {
    AWS_LOGSTREAM_ERROR("GetDataRetrievalPolicy", "Required field: AccountId, is not set");
    return GetDataRetrievalPolicyOutcome(Aws::Client::AWSError<GlacierErrors>(GlacierErrors::MISSING_PARAMETER, "MISSING_PARAMETER", "Missing required field [AccountId]", false));
  }
  if (request.GetAccountId().size() != 12 || request.GetAccountId().find_first_not_of("0123456789") != Aws::String::npos)
  {
    AWS_LOGSTREAM_ERROR("GetDataRetrievalPolicy", "Required field: AccountId has invalid value");
    return GetDataRetrievalPolicyOutcome(Aws::Client::AWSError<GlacierErrors>(GlacierErrors::INVALID_PARAMETER_VALUE, "INVALID_PARAMETER", "AccountId is invalid", false));
  }
  ResolveEndpointOutcome endpointResolutionOutcome = m_endpointProvider->ResolveEndpoint(request.GetEndpointContextParams());
  AWS_OPERATION_CHECK_SUCCESS(endpointResolutionOutcome, GetDataRetrievalPolicy, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE, endpointResolutionOutcome.GetError().GetMessage());
  endpointResolutionOutcome.GetResult().AddPathSegment(request.GetAccountId());
  endpointResolutionOutcome.GetResult().AddPathSegments("/policies/data-retrieval");
  return GetDataRetrievalPolicyOutcome(MakeRequest(request, endpointResolutionOutcome.GetResult(), Aws::Http::HttpMethod::HTTP_GET, Aws::Auth::SIGV4_SIGNER));
}

GetDataRetrievalPolicyOutcomeCallable GlacierClient::GetDataRetrievalPolicyCallable(const GetDataRetrievalPolicyRequest& request) const
{
  AWS_MAKE_CALLABLE_OPERATION(GetDataRetrievalPolicy, request, m_executor.get());
}

void GlacierClient::GetDataRetrievalPolicyAsync(const GetDataRetrievalPolicyRequest& request, const GetDataRetrievalPolicyResponseReceivedHandler& handler, const std::shared_ptr<const Aws::Client::AsyncCallerContext>& context) const
{
  AWS_MAKE_ASYNC_OPERATION(GetDataRetrievalPolicy, request, handler, context, m_executor.get());
}

GetJobOutputOutcome GlacierClient::GetJobOutput(const GetJobOutputRequest& request) const
{
  AWS_OPERATION_CHECK_PTR(m_endpointProvider, GetJobOutput, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE);
  if (!request.AccountIdHasBeenSet())
  {
    AWS_LOGSTREAM_ERROR("GetJobOutput", "Required field: AccountId, is not set");
    return GetJobOutputOutcome(Aws::Client::AWSError<GlacierErrors>(GlacierErrors::MISSING_PARAMETER, "MISSING_PARAMETER", "Missing required field [AccountId]", false));
  }
  if (!request.VaultNameHasBeenSet())
  {
    AWS_LOGSTREAM_ERROR("GetJobOutput", "Required field: VaultName, is not set");
    return GetJobOutputOutcome(Aws::Client::AWSError<GlacierErrors>(GlacierErrors::MISSING_PARAMETER, "MISSING_PARAMETER", "Missing required field [VaultName]", false));
  }
  if (!request.JobIdHasBeenSet())
  {
    AWS_LOGSTREAM_ERROR("GetJobOutput", "Required field: JobId, is not set");
    return GetJobOutputOutcome(Aws::Client::AWSError<GlacierErrors>(GlacierErrors::MISSING_PARAMETER, "MISSING_PARAMETER", "Missing required field [JobId]", false));
  }
  if (request.GetAccountId().size() != 12 || request.GetAccountId().find_first_not_of("0123456789") != Aws::String::npos)
  {
    AWS_LOGSTREAM_ERROR("GetJobOutput", "Required field: AccountId has invalid value");
    return GetJobOutputOutcome(Aws::Client::AWSError<GlacierErrors>(GlacierErrors::INVALID_PARAMETER_VALUE, "INVALID_PARAMETER", "AccountId is invalid", false));
  }
  ResolveEndpointOutcome endpointResolutionOutcome = m_endpointProvider->ResolveEndpoint(request.GetEndpointContextParams());
  AWS_OPERATION_CHECK_SUCCESS(endpointResolutionOutcome, GetJobOutput, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE, endpointResolutionOutcome.GetError().GetMessage());
  endpointResolutionOutcome.GetResult().AddPathSegment(request.GetAccountId());
  endpointResolutionOutcome.GetResult().AddPathSegments("/vaults/");
  endpointResolutionOutcome.GetResult().AddPathSegment(request.GetVaultName());
  endpointResolutionOutcome.GetResult().AddPathSegments("/jobs/");
  endpointResolutionOutcome.GetResult().AddPathSegment(request.GetJobId());
  endpointResolutionOutcome.GetResult().AddPathSegments("/output");
  return GetJobOutputOutcome(MakeRequestWithUnparsedResponse(request, endpointResolutionOutcome.GetResult(), Aws::Http::HttpMethod::HTTP_GET));
}

GetJobOutputOutcomeCallable GlacierClient::GetJobOutputCallable(const GetJobOutputRequest& request) const
{
  AWS_MAKE_CALLABLE_OPERATION(GetJobOutput, request, m_executor.get());
}

void GlacierClient::GetJobOutputAsync(const GetJobOutputRequest& request, const GetJobOutputResponseReceivedHandler& handler, const std::shared_ptr<const Aws::Client::AsyncCallerContext>& context) const
{
  AWS_MAKE_ASYNC_OPERATION(GetJobOutput, request, handler, context, m_executor.get());
}

GetVaultAccessPolicyOutcome GlacierClient::GetVaultAccessPolicy(const GetVaultAccessPolicyRequest& request) const
{
  AWS_OPERATION_CHECK_PTR(m_endpointProvider, GetVaultAccessPolicy, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE);
  if (!request.AccountIdHasBeenSet())
  {
    AWS_LOGSTREAM_ERROR("GetVaultAccessPolicy", "Required field: AccountId, is not set");
    return GetVaultAccessPolicyOutcome(Aws::Client::AWSError<GlacierErrors>(GlacierErrors::MISSING_PARAMETER, "MISSING_PARAMETER", "Missing required field [AccountId]", false));
  }
  if (!request.VaultNameHasBeenSet())
  {
    AWS_LOGSTREAM_ERROR("GetVaultAccessPolicy", "Required field: VaultName, is not set");
    return GetVaultAccessPolicyOutcome(Aws::Client::AWSError<GlacierErrors>(GlacierErrors::MISSING_PARAMETER, "MISSING_PARAMETER", "Missing required field [VaultName]", false));
  }
  if (request.GetAccountId().size() != 12 || request.GetAccountId().find_first_not_of("0123456789") != Aws::String::npos)
  {
    AWS_LOGSTREAM_ERROR("GetVaultAccessPolicy", "Required field: AccountId has invalid value");
    return GetVaultAccessPolicyOutcome(Aws::Client::AWSError<GlacierErrors>(GlacierErrors::INVALID_PARAMETER_VALUE, "INVALID_PARAMETER", "AccountId is invalid", false));
  }
  ResolveEndpointOutcome endpointResolutionOutcome = m_endpointProvider->ResolveEndpoint(request.GetEndpointContextParams());
  AWS_OPERATION_CHECK_SUCCESS(endpointResolutionOutcome, GetVaultAccessPolicy, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE, endpointResolutionOutcome.GetError().GetMessage());
  endpointResolutionOutcome.GetResult().AddPathSegment(request.GetAccountId());
  endpointResolutionOutcome.GetResult().AddPathSegments("/vaults/");
  endpointResolutionOutcome.GetResult().AddPathSegment(request.GetVaultName());
  endpointResolutionOutcome.GetResult().AddPathSegments("/access-policy");
  return GetVaultAccessPolicyOutcome(MakeRequest(request, endpointResolutionOutcome.GetResult(), Aws::Http::HttpMethod::HTTP_GET, Aws::Auth::SIGV4_SIGNER));
}

GetVaultAccessPolicyOutcomeCallable GlacierClient::GetVaultAccessPolicyCallable(const GetVaultAccessPolicyRequest& request) const
{
  AWS_MAKE_CALLABLE_OPERATION(GetVaultAccessPolicy, request, m_executor.get());
}

void GlacierClient::GetVaultAccessPolicyAsync(const GetVaultAccessPolicyRequest& request, const GetVaultAccessPolicyResponseReceivedHandler& handler, const std::shared_ptr<const Aws::Client::AsyncCallerContext>& context) const
{
  AWS_MAKE_ASYNC_OPERATION(GetVaultAccessPolicy, request, handler, context, m_executor.get());
}

GetVaultLockOutcome GlacierClient::GetVaultLock(const GetVaultLockRequest& request) const
{
  AWS_OPERATION_CHECK_PTR(m_endpointProvider, GetVaultLock, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE);
  if (!request.AccountIdHasBeenSet())
  {
    AWS_LOGSTREAM_ERROR("GetVaultLock", "Required field: AccountId, is not set");
    return GetVaultLockOutcome(Aws::Client::AWSError<GlacierErrors>(GlacierErrors::MISSING_PARAMETER, "MISSING_PARAMETER", "Missing required field [AccountId]", false));
  }
  if (!request.VaultNameHasBeenSet())
  {
    AWS_LOGSTREAM_ERROR("GetVaultLock", "Required field: VaultName, is not set");
    return GetVaultLockOutcome(Aws::Client::AWSError<GlacierErrors>(GlacierErrors::MISSING_PARAMETER, "MISSING_PARAMETER", "Missing required field [VaultName]", false));
  }
  if (request.GetAccountId().size() != 12 || request.GetAccountId().find_first_not_of("0123456789") != Aws::String::npos)
  {
    AWS_LOGSTREAM_ERROR("GetVaultLock", "Required field: AccountId has invalid value");
    return GetVaultLockOutcome(Aws::Client::AWSError<GlacierErrors>(GlacierErrors::INVALID_PARAMETER_VALUE, "INVALID_PARAMETER", "AccountId is invalid", false));
  }
  ResolveEndpointOutcome endpointResolutionOutcome = m_endpointProvider->ResolveEndpoint(request.GetEndpointContextParams());
  AWS_OPERATION_CHECK_SUCCESS(endpointResolutionOutcome, GetVaultLock, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE, endpointResolutionOutcome.GetError().GetMessage());
  endpointResolutionOutcome.GetResult().AddPathSegment(request.GetAccountId());
  endpointResolutionOutcome.GetResult().AddPathSegments("/vaults/");
  endpointResolutionOutcome.GetResult().AddPathSegment(request.GetVaultName());
  endpointResolutionOutcome.GetResult().AddPathSegments("/lock-policy");
  return GetVaultLockOutcome(MakeRequest(request, endpointResolutionOutcome.GetResult(), Aws::Http::HttpMethod::HTTP_GET, Aws::Auth::SIGV4_SIGNER));
}

GetVaultLockOutcomeCallable GlacierClient::GetVaultLockCallable(const GetVaultLockRequest& request) const
{
  AWS_MAKE_CALLABLE_OPERATION(GetVaultLock, request, m_executor.get());
}

void GlacierClient::GetVaultLockAsync(const GetVaultLockRequest& request, const GetVaultLockResponseReceivedHandler& handler, const std::shared_ptr<const Aws::Client::AsyncCallerContext>& context) const
{
  AWS_MAKE_ASYNC_OPERATION(GetVaultLock, request, handler, context, m_executor.get());
}

GetVaultNotificationsOutcome GlacierClient::GetVaultNotifications(const GetVaultNotificationsRequest& request) const
{
  AWS_OPERATION_CHECK_PTR(m_endpointProvider, GetVaultNotifications, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE);
  if (!request.AccountIdHasBeenSet())
  {
    AWS_LOGSTREAM_ERROR("GetVaultNotifications", "Required field: AccountId, is not set");
    return GetVaultNotificationsOutcome(Aws::Client::AWSError<GlacierErrors>(GlacierErrors::MISSING_PARAMETER, "MISSING_PARAMETER", "Missing required field [AccountId]", false));
  }
  if (!request.VaultNameHasBeenSet())
  {
    AWS_LOGSTREAM_ERROR("GetVaultNotifications", "Required field: VaultName, is not set");
    return GetVaultNotificationsOutcome(Aws::Client::AWSError<GlacierErrors>(GlacierErrors::MISSING_PARAMETER, "MISSING_PARAMETER", "Missing required field [VaultName]", false));
  }
  if (request.GetAccountId().size() != 12 || request.GetAccountId().find_first_not_of("0123456789") != Aws::String::npos)
  {
    AWS_LOGSTREAM_ERROR("GetVaultNotifications", "Required field: AccountId has invalid value");
    return GetVaultNotificationsOutcome(Aws::Client::AWSError<GlacierErrors>(GlacierErrors::INVALID_PARAMETER_VALUE, "INVALID_PARAMETER", "AccountId is invalid", false));
  }
  ResolveEndpointOutcome endpointResolutionOutcome = m_endpointProvider->ResolveEndpoint(request.GetEndpointContextParams());
  AWS_OPERATION_CHECK_SUCCESS(endpointResolutionOutcome, GetVaultNotifications, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE, endpointResolutionOutcome.GetError().GetMessage());
  endpointResolutionOutcome.GetResult().AddPathSegment(request.GetAccountId());
  endpointResolutionOutcome.GetResult().AddPathSegments("/vaults/");
  endpointResolutionOutcome.GetResult().AddPathSegment(request.GetVaultName());
  endpointResolutionOutcome.GetResult().AddPathSegments("/notification-configuration");
  return GetVaultNotificationsOutcome(MakeRequest(request, endpointResolutionOutcome.GetResult(), Aws::Http::HttpMethod::HTTP_GET, Aws::Auth::SIGV4_SIGNER));
}

GetVaultNotificationsOutcomeCallable GlacierClient::GetVaultNotificationsCallable(const GetVaultNotificationsRequest& request) const
{
  AWS_MAKE_CALLABLE_OPERATION(GetVaultNotifications, request, m_executor.get());
}

void GlacierClient::GetVaultNotificationsAsync(const GetVaultNotificationsRequest& request, const GetVaultNotificationsResponseReceivedHandler& handler, const std::shared_ptr<const Aws::Client::AsyncCallerContext>& context) const
{
  AWS_MAKE_ASYNC_OPERATION(GetVaultNotifications, request, handler, context, m_executor.get());
}

InitiateJobOutcome GlacierClient::InitiateJob(const InitiateJobRequest& request) const
{
  AWS_OPERATION_CHECK_PTR(m_endpointProvider, InitiateJob, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE);
  if (!request.AccountIdHasBeenSet())
  {
    AWS_LOGSTREAM_ERROR("InitiateJob", "Required field: AccountId, is not set");
    return InitiateJobOutcome(Aws::Client::AWSError<GlacierErrors>(GlacierErrors::MISSING_PARAMETER, "MISSING_PARAMETER", "Missing required field [AccountId]", false));
  }
  if (!request.VaultNameHasBeenSet())
  {
    AWS_LOGSTREAM_ERROR("InitiateJob", "Required field: VaultName, is not set");
    return InitiateJobOutcome(Aws::Client::AWSError<GlacierErrors>(GlacierErrors::MISSING_PARAMETER, "MISSING_PARAMETER", "Missing required field [VaultName]", false));
  }
  if (request.GetAccountId().size() != 12 || request.GetAccountId().find_first_not_of("0123456789") != Aws::String::npos)
  {
    AWS_LOGSTREAM_ERROR("InitiateJob", "Required field: AccountId has invalid value");
    return InitiateJobOutcome(Aws::Client::AWSError<GlacierErrors>(GlacierErrors::INVALID_PARAMETER_VALUE, "INVALID_PARAMETER", "AccountId is invalid", false));
  }
  ResolveEndpointOutcome endpointResolutionOutcome = m_endpointProvider->ResolveEndpoint(request.GetEndpointContextParams());
  AWS_OPERATION_CHECK_SUCCESS(endpointResolutionOutcome, InitiateJob, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE, endpointResolutionOutcome.GetError().GetMessage());
  endpointResolutionOutcome.GetResult().AddPathSegment(request.GetAccountId());
  endpointResolutionOutcome.GetResult().AddPathSegments("/vaults/");
  endpointResolutionOutcome.GetResult().AddPathSegment(request.GetVaultName());
  endpointResolutionOutcome.GetResult().AddPathSegments("/jobs");
  return InitiateJobOutcome(MakeRequest(request, endpointResolutionOutcome.GetResult(), Aws::Http::HttpMethod::HTTP_POST, Aws::Auth::SIGV4_SIGNER));
}

InitiateJobOutcomeCallable GlacierClient::InitiateJobCallable(const InitiateJobRequest& request) const
{
  AWS_MAKE_CALLABLE_OPERATION(InitiateJob, request, m_executor.get());
}

void GlacierClient::InitiateJobAsync(const InitiateJobRequest& request, const InitiateJobResponseReceivedHandler& handler, const std::shared_ptr<const Aws::Client::AsyncCallerContext>& context) const
{
  AWS_MAKE_ASYNC_OPERATION(InitiateJob, request, handler, context, m_executor.get());
}

InitiateMultipartUploadOutcome GlacierClient::InitiateMultipartUpload(const InitiateMultipartUploadRequest& request) const
{
  AWS_OPERATION_CHECK_PTR(m_endpointProvider, InitiateMultipartUpload, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE);
  if (!request.AccountIdHasBeenSet())
  {
    AWS_LOGSTREAM_ERROR("InitiateMultipartUpload", "Required field: AccountId, is not set");
    return InitiateMultipartUploadOutcome(Aws::Client::AWSError<GlacierErrors>(GlacierErrors::MISSING_PARAMETER, "MISSING_PARAMETER", "Missing required field [AccountId]", false));
  }
  if (!request.VaultNameHasBeenSet())
  {
    AWS_LOGSTREAM_ERROR("InitiateMultipartUpload", "Required field: VaultName, is not set");
    return InitiateMultipartUploadOutcome(Aws::Client::AWSError<GlacierErrors>(GlacierErrors::MISSING_PARAMETER, "MISSING_PARAMETER", "Missing required field [VaultName]", false));
  }
  if (request.GetAccountId().size() != 12 || request.GetAccountId().find_first_not_of("0123456789") != Aws::String::npos)
  {
    AWS_LOGSTREAM_ERROR("InitiateMultipartUpload", "Required field: AccountId has invalid value");
    return InitiateMultipartUploadOutcome(Aws::Client::AWSError<GlacierErrors>(GlacierErrors::INVALID_PARAMETER_VALUE, "INVALID_PARAMETER", "AccountId is invalid", false));
  }
  ResolveEndpointOutcome endpointResolutionOutcome = m_endpointProvider->ResolveEndpoint(request.GetEndpointContextParams());
  AWS_OPERATION_CHECK_SUCCESS(endpointResolutionOutcome, InitiateMultipartUpload, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE, endpointResolutionOutcome.GetError().GetMessage());
  endpointResolutionOutcome.GetResult().AddPathSegment(request.GetAccountId());
  endpointResolutionOutcome.GetResult().AddPathSegments("/vaults/");
  endpointResolutionOutcome.GetResult().AddPathSegment(request.GetVaultName());
  endpointResolutionOutcome.GetResult().AddPathSegments("/multipart-uploads");
  return InitiateMultipartUploadOutcome(MakeRequest(request, endpointResolutionOutcome.GetResult(), Aws::Http::HttpMethod::HTTP_POST, Aws::Auth::SIGV4_SIGNER));
}

InitiateMultipartUploadOutcomeCallable GlacierClient::InitiateMultipartUploadCallable(const InitiateMultipartUploadRequest& request) const
{
  AWS_MAKE_CALLABLE_OPERATION(InitiateMultipartUpload, request, m_executor.get());
}

void GlacierClient::InitiateMultipartUploadAsync(const InitiateMultipartUploadRequest& request, const InitiateMultipartUploadResponseReceivedHandler& handler, const std::shared_ptr<const Aws::Client::AsyncCallerContext>& context) const
{
  AWS_MAKE_ASYNC_OPERATION(InitiateMultipartUpload, request, handler, context, m_executor.get());
}

InitiateVaultLockOutcome GlacierClient::InitiateVaultLock(const InitiateVaultLockRequest& request) const
{
  AWS_OPERATION_CHECK_PTR(m_endpointProvider, InitiateVaultLock, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE);
  if (!request.AccountIdHasBeenSet())
  {
    AWS_LOGSTREAM_ERROR("InitiateVaultLock", "Required field: AccountId, is not set");
    return InitiateVaultLockOutcome(Aws::Client::AWSError<GlacierErrors>(GlacierErrors::MISSING_PARAMETER, "MISSING_PARAMETER", "Missing required field [AccountId]", false));
  }
  if (!request.VaultNameHasBeenSet())
  {
    AWS_LOGSTREAM_ERROR("InitiateVaultLock", "Required field: VaultName, is not set");
    return InitiateVaultLockOutcome(Aws::Client::AWSError<GlacierErrors>(GlacierErrors::MISSING_PARAMETER, "MISSING_PARAMETER", "Missing required field [VaultName]", false));
  }
  if (request.GetAccountId().size() != 12 || request.GetAccountId().find_first_not_of("0123456789") != Aws::String::npos)
  {
    AWS_LOGSTREAM_ERROR("InitiateVaultLock", "Required field: AccountId has invalid value");
    return InitiateVaultLockOutcome(Aws::Client::AWSError<GlacierErrors>(GlacierErrors::INVALID_PARAMETER_VALUE, "INVALID_PARAMETER", "AccountId is invalid", false));
  }
  ResolveEndpointOutcome endpointResolutionOutcome = m_endpointProvider->ResolveEndpoint(request.GetEndpointContextParams());
  AWS_OPERATION_CHECK_SUCCESS(endpointResolutionOutcome, InitiateVaultLock, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE, endpointResolutionOutcome.GetError().GetMessage());
  endpointResolutionOutcome.GetResult().AddPathSegment(request.GetAccountId());
  endpointResolutionOutcome.GetResult().AddPathSegments("/vaults/");
  endpointResolutionOutcome.GetResult().AddPathSegment(request.GetVaultName());
  endpointResolutionOutcome.GetResult().AddPathSegments("/lock-policy");
  return InitiateVaultLockOutcome(MakeRequest(request, endpointResolutionOutcome.GetResult(), Aws::Http::HttpMethod::HTTP_POST, Aws::Auth::SIGV4_SIGNER));
}

InitiateVaultLockOutcomeCallable GlacierClient::InitiateVaultLockCallable(const InitiateVaultLockRequest& request) const
{
  AWS_MAKE_CALLABLE_OPERATION(InitiateVaultLock, request, m_executor.get());
}

void GlacierClient::InitiateVaultLockAsync(const InitiateVaultLockRequest& request, const InitiateVaultLockResponseReceivedHandler& handler, const std::shared_ptr<const Aws::Client::AsyncCallerContext>& context) const
{
  AWS_MAKE_ASYNC_OPERATION(InitiateVaultLock, request, handler, context, m_executor.get());
}

ListJobsOutcome GlacierClient::ListJobs(const ListJobsRequest& request) const
{
  AWS_OPERATION_CHECK_PTR(m_endpointProvider, ListJobs, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE);
  if (!request.AccountIdHasBeenSet())
  {
    AWS_LOGSTREAM_ERROR("ListJobs", "Required field: AccountId, is not set");
    return ListJobsOutcome(Aws::Client::AWSError<GlacierErrors>(GlacierErrors::MISSING_PARAMETER, "MISSING_PARAMETER", "Missing required field [AccountId]", false));
  }
  if (!request.VaultNameHasBeenSet())
  {
    AWS_LOGSTREAM_ERROR("ListJobs", "Required field: VaultName, is not set");
    return ListJobsOutcome(Aws::Client::AWSError<GlacierErrors>(GlacierErrors::MISSING_PARAMETER, "MISSING_PARAMETER", "Missing required field [VaultName]", false));
  }
  if (request.GetAccountId().size() != 12 || request.GetAccountId().find_first_not_of("0123456789") != Aws::String::npos)
  {
    AWS_LOGSTREAM_ERROR("ListJobs", "Required field: AccountId has invalid value");
    return ListJobsOutcome(Aws::Client::AWSError<GlacierErrors>(GlacierErrors::INVALID_PARAMETER_VALUE, "INVALID_PARAMETER", "AccountId is invalid", false));
  }
  ResolveEndpointOutcome endpointResolutionOutcome = m_endpointProvider->ResolveEndpoint(request.GetEndpointContextParams());
  AWS_OPERATION_CHECK_SUCCESS(endpointResolutionOutcome, ListJobs, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE, endpointResolutionOutcome.GetError().GetMessage());
  endpointResolutionOutcome.GetResult().AddPathSegment(request.GetAccountId());
  endpointResolutionOutcome.GetResult().AddPathSegments("/vaults/");
  endpointResolutionOutcome.GetResult().AddPathSegment(request.GetVaultName());
  endpointResolutionOutcome.GetResult().AddPathSegments("/jobs");
  return ListJobsOutcome(MakeRequest(request, endpointResolutionOutcome.GetResult(), Aws::Http::HttpMethod::HTTP_GET, Aws::Auth::SIGV4_SIGNER));
}

ListJobsOutcomeCallable GlacierClient::ListJobsCallable(const ListJobsRequest& request) const
{
  AWS_MAKE_CALLABLE_OPERATION(ListJobs, request, m_executor.get());
}

void GlacierClient::ListJobsAsync(const ListJobsRequest& request, const ListJobsResponseReceivedHandler& handler, const std::shared_ptr<const Aws::Client::AsyncCallerContext>& context) const
{
  AWS_MAKE_ASYNC_OPERATION(ListJobs, request, handler, context, m_executor.get());
}

ListMultipartUploadsOutcome GlacierClient::ListMultipartUploads(const ListMultipartUploadsRequest& request) const
{
  AWS_OPERATION_CHECK_PTR(m_endpointProvider, ListMultipartUploads, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE);
  if (!request.AccountIdHasBeenSet())
  {
    AWS_LOGSTREAM_ERROR("ListMultipartUploads", "Required field: AccountId, is not set");
    return ListMultipartUploadsOutcome(Aws::Client::AWSError<GlacierErrors>(GlacierErrors::MISSING_PARAMETER, "MISSING_PARAMETER", "Missing required field [AccountId]", false));
  }
  if (!request.VaultNameHasBeenSet())
  {
    AWS_LOGSTREAM_ERROR("ListMultipartUploads", "Required field: VaultName, is not set");
    return ListMultipartUploadsOutcome(Aws::Client::AWSError<GlacierErrors>(GlacierErrors::MISSING_PARAMETER, "MISSING_PARAMETER", "Missing required field [VaultName]", false));
  }
  if (request.GetAccountId().size() != 12 || request.GetAccountId().find_first_not_of("0123456789") != Aws::String::npos)
  {
    AWS_LOGSTREAM_ERROR("ListMultipartUploads", "Required field: AccountId has invalid value");
    return ListMultipartUploadsOutcome(Aws::Client::AWSError<GlacierErrors>(GlacierErrors::INVALID_PARAMETER_VALUE, "INVALID_PARAMETER", "AccountId is invalid", false));
  }
  ResolveEndpointOutcome endpointResolutionOutcome = m_endpointProvider->ResolveEndpoint(request.GetEndpointContextParams());
  AWS_OPERATION_CHECK_SUCCESS(endpointResolutionOutcome, ListMultipartUploads, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE, endpointResolutionOutcome.GetError().GetMessage());
  endpointResolutionOutcome.GetResult().AddPathSegment(request.GetAccountId());
  endpointResolutionOutcome.GetResult().AddPathSegments("/vaults/");
  endpointResolutionOutcome.GetResult().AddPathSegment(request.GetVaultName());
  endpointResolutionOutcome.GetResult().AddPathSegments("/multipart-uploads");
  return ListMultipartUploadsOutcome(MakeRequest(request, endpointResolutionOutcome.GetResult(), Aws::Http::HttpMethod::HTTP_GET, Aws::Auth::SIGV4_SIGNER));
}

ListMultipartUploadsOutcomeCallable GlacierClient::ListMultipartUploadsCallable(const ListMultipartUploadsRequest& request) const
{
  AWS_MAKE_CALLABLE_OPERATION(ListMultipartUploads, request, m_executor.get());
}

void GlacierClient::ListMultipartUploadsAsync(const ListMultipartUploadsRequest& request, const ListMultipartUploadsResponseReceivedHandler& handler, const std::shared_ptr<const Aws::Client::AsyncCallerContext>& context) const
{
  AWS_MAKE_ASYNC_OPERATION(ListMultipartUploads, request, handler, context, m_executor.get());
}

ListPartsOutcome GlacierClient::ListParts(const ListPartsRequest& request) const
{
  AWS_OPERATION_CHECK_PTR(m_endpointProvider, ListParts, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE);
  if (!request.AccountIdHasBeenSet())
  {
    AWS_LOGSTREAM_ERROR("ListParts", "Required field: AccountId, is not set");
    return ListPartsOutcome(Aws::Client::AWSError<GlacierErrors>(GlacierErrors::MISSING_PARAMETER, "MISSING_PARAMETER", "Missing required field [AccountId]", false));
  }
  if (!request.VaultNameHasBeenSet())
  {
    AWS_LOGSTREAM_ERROR("ListParts", "Required field: VaultName, is not set");
    return ListPartsOutcome(Aws::Client::AWSError<GlacierErrors>(GlacierErrors::MISSING_PARAMETER, "MISSING_PARAMETER", "Missing required field [VaultName]", false));
  }
  if (!request.UploadIdHasBeenSet())
  {
    AWS_LOGSTREAM_ERROR("ListParts", "Required field: UploadId, is not set");
    return ListPartsOutcome(Aws::Client::AWSError<GlacierErrors>(GlacierErrors::MISSING_PARAMETER, "MISSING_PARAMETER", "Missing required field [UploadId]", false));
  }
  if (request.GetAccountId().size() != 12 || request.GetAccountId().find_first_not_of("0123456789") != Aws::String::npos)
  {
    AWS_LOGSTREAM_ERROR("ListParts", "Required field: AccountId has invalid value");
    return ListPartsOutcome(Aws::Client::AWSError<GlacierErrors>(GlacierErrors::INVALID_PARAMETER_VALUE, "INVALID_PARAMETER", "AccountId is invalid", false));
  }
  ResolveEndpointOutcome endpointResolutionOutcome = m_endpointProvider->ResolveEndpoint(request.GetEndpointContextParams());
  AWS_OPERATION_CHECK_SUCCESS(endpointResolutionOutcome, ListParts, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE, endpointResolutionOutcome.GetError().GetMessage());
  endpointResolutionOutcome.GetResult().AddPathSegment(request.GetAccountId());
  endpointResolutionOutcome.GetResult().AddPathSegments("/vaults/");
  endpointResolutionOutcome.GetResult().AddPathSegment(request.GetVaultName());
  endpointResolutionOutcome.GetResult().AddPathSegments("/multipart-uploads/");
  endpointResolutionOutcome.GetResult().AddPathSegment(request.GetUploadId());
  return ListPartsOutcome(MakeRequest(request, endpointResolutionOutcome.GetResult(), Aws::Http::HttpMethod::HTTP_GET, Aws::Auth::SIGV4_SIGNER));
}

ListPartsOutcomeCallable GlacierClient::ListPartsCallable(const ListPartsRequest& request) const
{
  AWS_MAKE_CALLABLE_OPERATION(ListParts, request, m_executor.get());
}

void GlacierClient::ListPartsAsync(const ListPartsRequest& request, const ListPartsResponseReceivedHandler& handler, const std::shared_ptr<const Aws::Client::AsyncCallerContext>& context) const
{
  AWS_MAKE_ASYNC_OPERATION(ListParts, request, handler, context, m_executor.get());
}

ListProvisionedCapacityOutcome GlacierClient::ListProvisionedCapacity(const ListProvisionedCapacityRequest& request) const
{
  AWS_OPERATION_CHECK_PTR(m_endpointProvider, ListProvisionedCapacity, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE);
  if (!request.AccountIdHasBeenSet())
  {
    AWS_LOGSTREAM_ERROR("ListProvisionedCapacity", "Required field: AccountId, is not set");
    return ListProvisionedCapacityOutcome(Aws::Client::AWSError<GlacierErrors>(GlacierErrors::MISSING_PARAMETER, "MISSING_PARAMETER", "Missing required field [AccountId]", false));
  }
  if (request.GetAccountId().size() != 12 || request.GetAccountId().find_first_not_of("0123456789") != Aws::String::npos)
  {
    AWS_LOGSTREAM_ERROR("ListProvisionedCapacity", "Required field: AccountId has invalid value");
    return ListProvisionedCapacityOutcome(Aws::Client::AWSError<GlacierErrors>(GlacierErrors::INVALID_PARAMETER_VALUE, "INVALID_PARAMETER", "AccountId is invalid", false));
  }
  ResolveEndpointOutcome endpointResolutionOutcome = m_endpointProvider->ResolveEndpoint(request.GetEndpointContextParams());
  AWS_OPERATION_CHECK_SUCCESS(endpointResolutionOutcome, ListProvisionedCapacity, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE, endpointResolutionOutcome.GetError().GetMessage());
  endpointResolutionOutcome.GetResult().AddPathSegment(request.GetAccountId());
  endpointResolutionOutcome.GetResult().AddPathSegments("/provisioned-capacity");
  return ListProvisionedCapacityOutcome(MakeRequest(request, endpointResolutionOutcome.GetResult(), Aws::Http::HttpMethod::HTTP_GET, Aws::Auth::SIGV4_SIGNER));
}

ListProvisionedCapacityOutcomeCallable GlacierClient::ListProvisionedCapacityCallable(const ListProvisionedCapacityRequest& request) const
{
  AWS_MAKE_CALLABLE_OPERATION(ListProvisionedCapacity, request, m_executor.get());
}

void GlacierClient::ListProvisionedCapacityAsync(const ListProvisionedCapacityRequest& request, const ListProvisionedCapacityResponseReceivedHandler& handler, const std::shared_ptr<const Aws::Client::AsyncCallerContext>& context) const
{
  AWS_MAKE_ASYNC_OPERATION(ListProvisionedCapacity, request, handler, context, m_executor.get());
}

ListTagsForVaultOutcome GlacierClient::ListTagsForVault(const ListTagsForVaultRequest& request) const
{
  AWS_OPERATION_CHECK_PTR(m_endpointProvider, ListTagsForVault, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE);
  if (!request.AccountIdHasBeenSet())
  {
    AWS_LOGSTREAM_ERROR("ListTagsForVault", "Required field: AccountId, is not set");
    return ListTagsForVaultOutcome(Aws::Client::AWSError<GlacierErrors>(GlacierErrors::MISSING_PARAMETER, "MISSING_PARAMETER", "Missing required field [AccountId]", false));
  }
  if (!request.VaultNameHasBeenSet())
  {
    AWS_LOGSTREAM_ERROR("ListTagsForVault", "Required field: VaultName, is not set");
    return ListTagsForVaultOutcome(Aws::Client::AWSError<GlacierErrors>(GlacierErrors::MISSING_PARAMETER, "MISSING_PARAMETER", "Missing required field [VaultName]", false));
  }
  if (request.GetAccountId().size() != 12 || request.GetAccountId().find_first_not_of("0123456789") != Aws::String::npos)
  {
    AWS_LOGSTREAM_ERROR("ListTagsForVault", "Required field: AccountId has invalid value");
    return ListTagsForVaultOutcome(Aws::Client::AWSError<GlacierErrors>(GlacierErrors::INVALID_PARAMETER_VALUE, "INVALID_PARAMETER", "AccountId is invalid", false));
  }
  ResolveEndpointOutcome endpointResolutionOutcome = m_endpointProvider->ResolveEndpoint(request.GetEndpointContextParams());
  AWS_OPERATION_CHECK_SUCCESS(endpointResolutionOutcome, ListTagsForVault, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE, endpointResolutionOutcome.GetError().GetMessage());
  endpointResolutionOutcome.GetResult().AddPathSegment(request.GetAccountId());
  endpointResolutionOutcome.GetResult().AddPathSegments("/vaults/");
  endpointResolutionOutcome.GetResult().AddPathSegment(request.GetVaultName());
  endpointResolutionOutcome.GetResult().AddPathSegments("/tags");
  return ListTagsForVaultOutcome(MakeRequest(request, endpointResolutionOutcome.GetResult(), Aws::Http::HttpMethod::HTTP_GET, Aws::Auth::SIGV4_SIGNER));
}

ListTagsForVaultOutcomeCallable GlacierClient::ListTagsForVaultCallable(const ListTagsForVaultRequest& request) const
{
  AWS_MAKE_CALLABLE_OPERATION(ListTagsForVault, request, m_executor.get());
}

void GlacierClient::ListTagsForVaultAsync(const ListTagsForVaultRequest& request, const ListTagsForVaultResponseReceivedHandler& handler, const std::shared_ptr<const Aws::Client::AsyncCallerContext>& context) const
{
  AWS_MAKE_ASYNC_OPERATION(ListTagsForVault, request, handler, context, m_executor.get());
}

ListVaultsOutcome GlacierClient::ListVaults(const ListVaultsRequest& request) const
{
  AWS_OPERATION_CHECK_PTR(m_endpointProvider, ListVaults, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE);
  if (!request.AccountIdHasBeenSet())
  {
    AWS_LOGSTREAM_ERROR("ListVaults", "Required field: AccountId, is not set");
    return ListVaultsOutcome(Aws::Client::AWSError<GlacierErrors>(GlacierErrors::MISSING_PARAMETER, "MISSING_PARAMETER", "Missing required field [AccountId]", false));
  }
  if (request.GetAccountId().size() != 12 || request.GetAccountId().find_first_not_of("0123456789") != Aws::String::npos)
  {
    AWS_LOGSTREAM_ERROR("ListVaults", "Required field: AccountId has invalid value");
    return ListVaultsOutcome(Aws::Client::AWSError<GlacierErrors>(GlacierErrors::INVALID_PARAMETER_VALUE, "INVALID_PARAMETER", "AccountId is invalid", false));
  }
  ResolveEndpointOutcome endpointResolutionOutcome = m_endpointProvider->ResolveEndpoint(request.GetEndpointContextParams());
  AWS_OPERATION_CHECK_SUCCESS(endpointResolutionOutcome, ListVaults, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE, endpointResolutionOutcome.GetError().GetMessage());
  endpointResolutionOutcome.GetResult().AddPathSegment(request.GetAccountId());
  endpointResolutionOutcome.GetResult().AddPathSegments("/vaults");
  return ListVaultsOutcome(MakeRequest(request, endpointResolutionOutcome.GetResult(), Aws::Http::HttpMethod::HTTP_GET, Aws::Auth::SIGV4_SIGNER));
}

ListVaultsOutcomeCallable GlacierClient::ListVaultsCallable(const ListVaultsRequest& request) const
{
  AWS_MAKE_CALLABLE_OPERATION(ListVaults, request, m_executor.get());
}

void GlacierClient::ListVaultsAsync(const ListVaultsRequest& request, const ListVaultsResponseReceivedHandler& handler, const std::shared_ptr<const Aws::Client::AsyncCallerContext>& context) const
{
  AWS_MAKE_ASYNC_OPERATION(ListVaults, request, handler, context, m_executor.get());
}

PurchaseProvisionedCapacityOutcome GlacierClient::PurchaseProvisionedCapacity(const PurchaseProvisionedCapacityRequest& request) const
{
  AWS_OPERATION_CHECK_PTR(m_endpointProvider, PurchaseProvisionedCapacity, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE);
  if (!request.AccountIdHasBeenSet())
  {
    AWS_LOGSTREAM_ERROR("PurchaseProvisionedCapacity", "Required field: AccountId, is not set");
    return PurchaseProvisionedCapacityOutcome(Aws::Client::AWSError<GlacierErrors>(GlacierErrors::MISSING_PARAMETER, "MISSING_PARAMETER", "Missing required field [AccountId]", false));
  }
  if (request.GetAccountId().size() != 12 || request.GetAccountId().find_first_not_of("0123456789") != Aws::String::npos)
  {
    AWS_LOGSTREAM_ERROR("PurchaseProvisionedCapacity", "Required field: AccountId has invalid value");
    return PurchaseProvisionedCapacityOutcome(Aws::Client::AWSError<GlacierErrors>(GlacierErrors::INVALID_PARAMETER_VALUE, "INVALID_PARAMETER", "AccountId is invalid", false));
  }
  ResolveEndpointOutcome endpointResolutionOutcome = m_endpointProvider->ResolveEndpoint(request.GetEndpointContextParams());
  AWS_OPERATION_CHECK_SUCCESS(endpointResolutionOutcome, PurchaseProvisionedCapacity, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE, endpointResolutionOutcome.GetError().GetMessage());
  endpointResolutionOutcome.GetResult().AddPathSegment(request.GetAccountId());
  endpointResolutionOutcome.GetResult().AddPathSegments("/provisioned-capacity");
  return PurchaseProvisionedCapacityOutcome(MakeRequest(request, endpointResolutionOutcome.GetResult(), Aws::Http::HttpMethod::HTTP_POST, Aws::Auth::SIGV4_SIGNER));
}

PurchaseProvisionedCapacityOutcomeCallable GlacierClient::PurchaseProvisionedCapacityCallable(const PurchaseProvisionedCapacityRequest& request) const
{
  AWS_MAKE_CALLABLE_OPERATION(PurchaseProvisionedCapacity, request, m_executor.get());
}

void GlacierClient::PurchaseProvisionedCapacityAsync(const PurchaseProvisionedCapacityRequest& request, const PurchaseProvisionedCapacityResponseReceivedHandler& handler, const std::shared_ptr<const Aws::Client::AsyncCallerContext>& context) const
{
  AWS_MAKE_ASYNC_OPERATION(PurchaseProvisionedCapacity, request, handler, context, m_executor.get());
}

RemoveTagsFromVaultOutcome GlacierClient::RemoveTagsFromVault(const RemoveTagsFromVaultRequest& request) const
{
  AWS_OPERATION_CHECK_PTR(m_endpointProvider, RemoveTagsFromVault, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE);
  if (!request.AccountIdHasBeenSet())
  {
    AWS_LOGSTREAM_ERROR("RemoveTagsFromVault", "Required field: AccountId, is not set");
    return RemoveTagsFromVaultOutcome(Aws::Client::AWSError<GlacierErrors>(GlacierErrors::MISSING_PARAMETER, "MISSING_PARAMETER", "Missing required field [AccountId]", false));
  }
  if (!request.VaultNameHasBeenSet())
  {
    AWS_LOGSTREAM_ERROR("RemoveTagsFromVault", "Required field: VaultName, is not set");
    return RemoveTagsFromVaultOutcome(Aws::Client::AWSError<GlacierErrors>(GlacierErrors::MISSING_PARAMETER, "MISSING_PARAMETER", "Missing required field [VaultName]", false));
  }
  if (request.GetAccountId().size() != 12 || request.GetAccountId().find_first_not_of("0123456789") != Aws::String::npos)
  {
    AWS_LOGSTREAM_ERROR("RemoveTagsFromVault", "Required field: AccountId has invalid value");
    return RemoveTagsFromVaultOutcome(Aws::Client::AWSError<GlacierErrors>(GlacierErrors::INVALID_PARAMETER_VALUE, "INVALID_PARAMETER", "AccountId is invalid", false));
  }
  ResolveEndpointOutcome endpointResolutionOutcome = m_endpointProvider->ResolveEndpoint(request.GetEndpointContextParams());
  AWS_OPERATION_CHECK_SUCCESS(endpointResolutionOutcome, RemoveTagsFromVault, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE, endpointResolutionOutcome.GetError().GetMessage());
  Aws::StringStream ss;
  endpointResolutionOutcome.GetResult().AddPathSegment(request.GetAccountId());
  endpointResolutionOutcome.GetResult().AddPathSegments("/vaults/");
  endpointResolutionOutcome.GetResult().AddPathSegment(request.GetVaultName());
  endpointResolutionOutcome.GetResult().AddPathSegments("/tags");
  ss.str("?operation=remove");
  endpointResolutionOutcome.GetResult().SetQueryString(ss.str());
  return RemoveTagsFromVaultOutcome(MakeRequest(request, endpointResolutionOutcome.GetResult(), Aws::Http::HttpMethod::HTTP_POST, Aws::Auth::SIGV4_SIGNER));
}

RemoveTagsFromVaultOutcomeCallable GlacierClient::RemoveTagsFromVaultCallable(const RemoveTagsFromVaultRequest& request) const
{
  AWS_MAKE_CALLABLE_OPERATION(RemoveTagsFromVault, request, m_executor.get());
}

void GlacierClient::RemoveTagsFromVaultAsync(const RemoveTagsFromVaultRequest& request, const RemoveTagsFromVaultResponseReceivedHandler& handler, const std::shared_ptr<const Aws::Client::AsyncCallerContext>& context) const
{
  AWS_MAKE_ASYNC_OPERATION(RemoveTagsFromVault, request, handler, context, m_executor.get());
}

SetDataRetrievalPolicyOutcome GlacierClient::SetDataRetrievalPolicy(const SetDataRetrievalPolicyRequest& request) const
{
  AWS_OPERATION_CHECK_PTR(m_endpointProvider, SetDataRetrievalPolicy, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE);
  if (!request.AccountIdHasBeenSet())
  {
    AWS_LOGSTREAM_ERROR("SetDataRetrievalPolicy", "Required field: AccountId, is not set");
    return SetDataRetrievalPolicyOutcome(Aws::Client::AWSError<GlacierErrors>(GlacierErrors::MISSING_PARAMETER, "MISSING_PARAMETER", "Missing required field [AccountId]", false));
  }
  if (request.GetAccountId().size() != 12 || request.GetAccountId().find_first_not_of("0123456789") != Aws::String::npos)
  {
    AWS_LOGSTREAM_ERROR("SetDataRetrievalPolicy", "Required field: AccountId has invalid value");
    return SetDataRetrievalPolicyOutcome(Aws::Client::AWSError<GlacierErrors>(GlacierErrors::INVALID_PARAMETER_VALUE, "INVALID_PARAMETER", "AccountId is invalid", false));
  }
  ResolveEndpointOutcome endpointResolutionOutcome = m_endpointProvider->ResolveEndpoint(request.GetEndpointContextParams());
  AWS_OPERATION_CHECK_SUCCESS(endpointResolutionOutcome, SetDataRetrievalPolicy, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE, endpointResolutionOutcome.GetError().GetMessage());
  endpointResolutionOutcome.GetResult().AddPathSegment(request.GetAccountId());
  endpointResolutionOutcome.GetResult().AddPathSegments("/policies/data-retrieval");
  return SetDataRetrievalPolicyOutcome(MakeRequest(request, endpointResolutionOutcome.GetResult(), Aws::Http::HttpMethod::HTTP_PUT, Aws::Auth::SIGV4_SIGNER));
}

SetDataRetrievalPolicyOutcomeCallable GlacierClient::SetDataRetrievalPolicyCallable(const SetDataRetrievalPolicyRequest& request) const
{
  AWS_MAKE_CALLABLE_OPERATION(SetDataRetrievalPolicy, request, m_executor.get());
}

void GlacierClient::SetDataRetrievalPolicyAsync(const SetDataRetrievalPolicyRequest& request, const SetDataRetrievalPolicyResponseReceivedHandler& handler, const std::shared_ptr<const Aws::Client::AsyncCallerContext>& context) const
{
  AWS_MAKE_ASYNC_OPERATION(SetDataRetrievalPolicy, request, handler, context, m_executor.get());
}

SetVaultAccessPolicyOutcome GlacierClient::SetVaultAccessPolicy(const SetVaultAccessPolicyRequest& request) const
{
  AWS_OPERATION_CHECK_PTR(m_endpointProvider, SetVaultAccessPolicy, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE);
  if (!request.AccountIdHasBeenSet())
  {
    AWS_LOGSTREAM_ERROR("SetVaultAccessPolicy", "Required field: AccountId, is not set");
    return SetVaultAccessPolicyOutcome(Aws::Client::AWSError<GlacierErrors>(GlacierErrors::MISSING_PARAMETER, "MISSING_PARAMETER", "Missing required field [AccountId]", false));
  }
  if (!request.VaultNameHasBeenSet())
  {
    AWS_LOGSTREAM_ERROR("SetVaultAccessPolicy", "Required field: VaultName, is not set");
    return SetVaultAccessPolicyOutcome(Aws::Client::AWSError<GlacierErrors>(GlacierErrors::MISSING_PARAMETER, "MISSING_PARAMETER", "Missing required field [VaultName]", false));
  }
  if (request.GetAccountId().size() != 12 || request.GetAccountId().find_first_not_of("0123456789") != Aws::String::npos)
  {
    AWS_LOGSTREAM_ERROR("SetVaultAccessPolicy", "Required field: AccountId has invalid value");
    return SetVaultAccessPolicyOutcome(Aws::Client::AWSError<GlacierErrors>(GlacierErrors::INVALID_PARAMETER_VALUE, "INVALID_PARAMETER", "AccountId is invalid", false));
  }
  ResolveEndpointOutcome endpointResolutionOutcome = m_endpointProvider->ResolveEndpoint(request.GetEndpointContextParams());
  AWS_OPERATION_CHECK_SUCCESS(endpointResolutionOutcome, SetVaultAccessPolicy, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE, endpointResolutionOutcome.GetError().GetMessage());
  endpointResolutionOutcome.GetResult().AddPathSegment(request.GetAccountId());
  endpointResolutionOutcome.GetResult().AddPathSegments("/vaults/");
  endpointResolutionOutcome.GetResult().AddPathSegment(request.GetVaultName());
  endpointResolutionOutcome.GetResult().AddPathSegments("/access-policy");
  return SetVaultAccessPolicyOutcome(MakeRequest(request, endpointResolutionOutcome.GetResult(), Aws::Http::HttpMethod::HTTP_PUT, Aws::Auth::SIGV4_SIGNER));
}

SetVaultAccessPolicyOutcomeCallable GlacierClient::SetVaultAccessPolicyCallable(const SetVaultAccessPolicyRequest& request) const
{
  AWS_MAKE_CALLABLE_OPERATION(SetVaultAccessPolicy, request, m_executor.get());
}

void GlacierClient::SetVaultAccessPolicyAsync(const SetVaultAccessPolicyRequest& request, const SetVaultAccessPolicyResponseReceivedHandler& handler, const std::shared_ptr<const Aws::Client::AsyncCallerContext>& context) const
{
  AWS_MAKE_ASYNC_OPERATION(SetVaultAccessPolicy, request, handler, context, m_executor.get());
}

SetVaultNotificationsOutcome GlacierClient::SetVaultNotifications(const SetVaultNotificationsRequest& request) const
{
  AWS_OPERATION_CHECK_PTR(m_endpointProvider, SetVaultNotifications, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE);
  if (!request.AccountIdHasBeenSet())
  {
    AWS_LOGSTREAM_ERROR("SetVaultNotifications", "Required field: AccountId, is not set");
    return SetVaultNotificationsOutcome(Aws::Client::AWSError<GlacierErrors>(GlacierErrors::MISSING_PARAMETER, "MISSING_PARAMETER", "Missing required field [AccountId]", false));
  }
  if (!request.VaultNameHasBeenSet())
  {
    AWS_LOGSTREAM_ERROR("SetVaultNotifications", "Required field: VaultName, is not set");
    return SetVaultNotificationsOutcome(Aws::Client::AWSError<GlacierErrors>(GlacierErrors::MISSING_PARAMETER, "MISSING_PARAMETER", "Missing required field [VaultName]", false));
  }
  if (request.GetAccountId().size() != 12 || request.GetAccountId().find_first_not_of("0123456789") != Aws::String::npos)
  {
    AWS_LOGSTREAM_ERROR("SetVaultNotifications", "Required field: AccountId has invalid value");
    return SetVaultNotificationsOutcome(Aws::Client::AWSError<GlacierErrors>(GlacierErrors::INVALID_PARAMETER_VALUE, "INVALID_PARAMETER", "AccountId is invalid", false));
  }
  ResolveEndpointOutcome endpointResolutionOutcome = m_endpointProvider->ResolveEndpoint(request.GetEndpointContextParams());
  AWS_OPERATION_CHECK_SUCCESS(endpointResolutionOutcome, SetVaultNotifications, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE, endpointResolutionOutcome.GetError().GetMessage());
  endpointResolutionOutcome.GetResult().AddPathSegment(request.GetAccountId());
  endpointResolutionOutcome.GetResult().AddPathSegments("/vaults/");
  endpointResolutionOutcome.GetResult().AddPathSegment(request.GetVaultName());
  endpointResolutionOutcome.GetResult().AddPathSegments("/notification-configuration");
  return SetVaultNotificationsOutcome(MakeRequest(request, endpointResolutionOutcome.GetResult(), Aws::Http::HttpMethod::HTTP_PUT, Aws::Auth::SIGV4_SIGNER));
}

SetVaultNotificationsOutcomeCallable GlacierClient::SetVaultNotificationsCallable(const SetVaultNotificationsRequest& request) const
{
  AWS_MAKE_CALLABLE_OPERATION(SetVaultNotifications, request, m_executor.get());
}

void GlacierClient::SetVaultNotificationsAsync(const SetVaultNotificationsRequest& request, const SetVaultNotificationsResponseReceivedHandler& handler, const std::shared_ptr<const Aws::Client::AsyncCallerContext>& context) const
{
  AWS_MAKE_ASYNC_OPERATION(SetVaultNotifications, request, handler, context, m_executor.get());
}

UploadArchiveOutcome GlacierClient::UploadArchive(const UploadArchiveRequest& request) const
{
  AWS_OPERATION_CHECK_PTR(m_endpointProvider, UploadArchive, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE);
  if (!request.VaultNameHasBeenSet())
  {
    AWS_LOGSTREAM_ERROR("UploadArchive", "Required field: VaultName, is not set");
    return UploadArchiveOutcome(Aws::Client::AWSError<GlacierErrors>(GlacierErrors::MISSING_PARAMETER, "MISSING_PARAMETER", "Missing required field [VaultName]", false));
  }
  if (!request.AccountIdHasBeenSet())
  {
    AWS_LOGSTREAM_ERROR("UploadArchive", "Required field: AccountId, is not set");
    return UploadArchiveOutcome(Aws::Client::AWSError<GlacierErrors>(GlacierErrors::MISSING_PARAMETER, "MISSING_PARAMETER", "Missing required field [AccountId]", false));
  }
  if (request.GetAccountId().size() != 12 || request.GetAccountId().find_first_not_of("0123456789") != Aws::String::npos)
  {
    AWS_LOGSTREAM_ERROR("UploadArchive", "Required field: AccountId has invalid value");
    return UploadArchiveOutcome(Aws::Client::AWSError<GlacierErrors>(GlacierErrors::INVALID_PARAMETER_VALUE, "INVALID_PARAMETER", "AccountId is invalid", false));
  }
  ResolveEndpointOutcome endpointResolutionOutcome = m_endpointProvider->ResolveEndpoint(request.GetEndpointContextParams());
  AWS_OPERATION_CHECK_SUCCESS(endpointResolutionOutcome, UploadArchive, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE, endpointResolutionOutcome.GetError().GetMessage());
  endpointResolutionOutcome.GetResult().AddPathSegment(request.GetAccountId());
  endpointResolutionOutcome.GetResult().AddPathSegments("/vaults/");
  endpointResolutionOutcome.GetResult().AddPathSegment(request.GetVaultName());
  endpointResolutionOutcome.GetResult().AddPathSegments("/archives");
  return UploadArchiveOutcome(MakeRequest(request, endpointResolutionOutcome.GetResult(), Aws::Http::HttpMethod::HTTP_POST, Aws::Auth::SIGV4_SIGNER));
}

UploadArchiveOutcomeCallable GlacierClient::UploadArchiveCallable(const UploadArchiveRequest& request) const
{
  AWS_MAKE_CALLABLE_OPERATION(UploadArchive, request, m_executor.get());
}

void GlacierClient::UploadArchiveAsync(const UploadArchiveRequest& request, const UploadArchiveResponseReceivedHandler& handler, const std::shared_ptr<const Aws::Client::AsyncCallerContext>& context) const
{
  AWS_MAKE_ASYNC_OPERATION(UploadArchive, request, handler, context, m_executor.get());
}

UploadMultipartPartOutcome GlacierClient::UploadMultipartPart(const UploadMultipartPartRequest& request) const
{
  AWS_OPERATION_CHECK_PTR(m_endpointProvider, UploadMultipartPart, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE);
  if (!request.AccountIdHasBeenSet())
  {
    AWS_LOGSTREAM_ERROR("UploadMultipartPart", "Required field: AccountId, is not set");
    return UploadMultipartPartOutcome(Aws::Client::AWSError<GlacierErrors>(GlacierErrors::MISSING_PARAMETER, "MISSING_PARAMETER", "Missing required field [AccountId]", false));
  }
  if (!request.VaultNameHasBeenSet())
  {
    AWS_LOGSTREAM_ERROR("UploadMultipartPart", "Required field: VaultName, is not set");
    return UploadMultipartPartOutcome(Aws::Client::AWSError<GlacierErrors>(GlacierErrors::MISSING_PARAMETER, "MISSING_PARAMETER", "Missing required field [VaultName]", false));
  }
  if (!request.UploadIdHasBeenSet())
  {
    AWS_LOGSTREAM_ERROR("UploadMultipartPart", "Required field: UploadId, is not set");
    return UploadMultipartPartOutcome(Aws::Client::AWSError<GlacierErrors>(GlacierErrors::MISSING_PARAMETER, "MISSING_PARAMETER", "Missing required field [UploadId]", false));
  }
  if (request.GetAccountId().size() != 12 || request.GetAccountId().find_first_not_of("0123456789") != Aws::String::npos)
  {
    AWS_LOGSTREAM_ERROR("UploadMultipartPart", "Required field: AccountId has invalid value");
    return UploadMultipartPartOutcome(Aws::Client::AWSError<GlacierErrors>(GlacierErrors::INVALID_PARAMETER_VALUE, "INVALID_PARAMETER", "AccountId is invalid", false));
  }
  ResolveEndpointOutcome endpointResolutionOutcome = m_endpointProvider->ResolveEndpoint(request.GetEndpointContextParams());
  AWS_OPERATION_CHECK_SUCCESS(endpointResolutionOutcome, UploadMultipartPart, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE, endpointResolutionOutcome.GetError().GetMessage());
  endpointResolutionOutcome.GetResult().AddPathSegment(request.GetAccountId());
  endpointResolutionOutcome.GetResult().AddPathSegments("/vaults/");
  endpointResolutionOutcome.GetResult().AddPathSegment(request.GetVaultName());
  endpointResolutionOutcome.GetResult().AddPathSegments("/multipart-uploads/");
  endpointResolutionOutcome.GetResult().AddPathSegment(request.GetUploadId());
  return UploadMultipartPartOutcome(MakeRequest(request, endpointResolutionOutcome.GetResult(), Aws::Http::HttpMethod::HTTP_PUT, Aws::Auth::SIGV4_SIGNER));
}

UploadMultipartPartOutcomeCallable GlacierClient::UploadMultipartPartCallable(const UploadMultipartPartRequest& request) const
{
  AWS_MAKE_CALLABLE_OPERATION(UploadMultipartPart, request, m_executor.get());
}

void GlacierClient::UploadMultipartPartAsync(const UploadMultipartPartRequest& request, const UploadMultipartPartResponseReceivedHandler& handler, const std::shared_ptr<const Aws::Client::AsyncCallerContext>& context) const
{
  AWS_MAKE_ASYNC_OPERATION(UploadMultipartPart, request, handler, context, m_executor.get());
}

