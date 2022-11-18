﻿/**
 * Copyright Amazon.com, Inc. or its affiliates. All Rights Reserved.
 * SPDX-License-Identifier: Apache-2.0.
 */

#include <aws/core/utils/Outcome.h>
#include <aws/core/auth/AWSAuthSigner.h>
#include <aws/core/client/CoreErrors.h>
#include <aws/core/client/RetryStrategy.h>
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

#include <aws/acm-pca/ACMPCAClient.h>
#include <aws/acm-pca/ACMPCAErrorMarshaller.h>
#include <aws/acm-pca/ACMPCAEndpointProvider.h>
#include <aws/acm-pca/model/CreateCertificateAuthorityRequest.h>
#include <aws/acm-pca/model/CreateCertificateAuthorityAuditReportRequest.h>
#include <aws/acm-pca/model/CreatePermissionRequest.h>
#include <aws/acm-pca/model/DeleteCertificateAuthorityRequest.h>
#include <aws/acm-pca/model/DeletePermissionRequest.h>
#include <aws/acm-pca/model/DeletePolicyRequest.h>
#include <aws/acm-pca/model/DescribeCertificateAuthorityRequest.h>
#include <aws/acm-pca/model/DescribeCertificateAuthorityAuditReportRequest.h>
#include <aws/acm-pca/model/GetCertificateRequest.h>
#include <aws/acm-pca/model/GetCertificateAuthorityCertificateRequest.h>
#include <aws/acm-pca/model/GetCertificateAuthorityCsrRequest.h>
#include <aws/acm-pca/model/GetPolicyRequest.h>
#include <aws/acm-pca/model/ImportCertificateAuthorityCertificateRequest.h>
#include <aws/acm-pca/model/IssueCertificateRequest.h>
#include <aws/acm-pca/model/ListCertificateAuthoritiesRequest.h>
#include <aws/acm-pca/model/ListPermissionsRequest.h>
#include <aws/acm-pca/model/ListTagsRequest.h>
#include <aws/acm-pca/model/PutPolicyRequest.h>
#include <aws/acm-pca/model/RestoreCertificateAuthorityRequest.h>
#include <aws/acm-pca/model/RevokeCertificateRequest.h>
#include <aws/acm-pca/model/TagCertificateAuthorityRequest.h>
#include <aws/acm-pca/model/UntagCertificateAuthorityRequest.h>
#include <aws/acm-pca/model/UpdateCertificateAuthorityRequest.h>

using namespace Aws;
using namespace Aws::Auth;
using namespace Aws::Client;
using namespace Aws::ACMPCA;
using namespace Aws::ACMPCA::Model;
using namespace Aws::Http;
using namespace Aws::Utils::Json;
using ResolveEndpointOutcome = Aws::Endpoint::ResolveEndpointOutcome;

const char* ACMPCAClient::SERVICE_NAME = "acm-pca";
const char* ACMPCAClient::ALLOCATION_TAG = "ACMPCAClient";

ACMPCAClient::ACMPCAClient(const ACMPCA::ACMPCAClientConfiguration& clientConfiguration,
                           std::shared_ptr<ACMPCAEndpointProviderBase> endpointProvider) :
  BASECLASS(clientConfiguration,
            Aws::MakeShared<AWSAuthV4Signer>(ALLOCATION_TAG,
                                             Aws::MakeShared<DefaultAWSCredentialsProviderChain>(ALLOCATION_TAG),
                                             SERVICE_NAME,
                                             Aws::Region::ComputeSignerRegion(clientConfiguration.region)),
            Aws::MakeShared<ACMPCAErrorMarshaller>(ALLOCATION_TAG)),
  m_clientConfiguration(clientConfiguration),
  m_executor(clientConfiguration.executor),
  m_endpointProvider(std::move(endpointProvider))
{
  init(m_clientConfiguration);
}

ACMPCAClient::ACMPCAClient(const AWSCredentials& credentials,
                           std::shared_ptr<ACMPCAEndpointProviderBase> endpointProvider,
                           const ACMPCA::ACMPCAClientConfiguration& clientConfiguration) :
  BASECLASS(clientConfiguration,
            Aws::MakeShared<AWSAuthV4Signer>(ALLOCATION_TAG,
                                             Aws::MakeShared<SimpleAWSCredentialsProvider>(ALLOCATION_TAG, credentials),
                                             SERVICE_NAME,
                                             Aws::Region::ComputeSignerRegion(clientConfiguration.region)),
            Aws::MakeShared<ACMPCAErrorMarshaller>(ALLOCATION_TAG)),
    m_clientConfiguration(clientConfiguration),
    m_executor(clientConfiguration.executor),
    m_endpointProvider(std::move(endpointProvider))
{
  init(m_clientConfiguration);
}

ACMPCAClient::ACMPCAClient(const std::shared_ptr<AWSCredentialsProvider>& credentialsProvider,
                           std::shared_ptr<ACMPCAEndpointProviderBase> endpointProvider,
                           const ACMPCA::ACMPCAClientConfiguration& clientConfiguration) :
  BASECLASS(clientConfiguration,
            Aws::MakeShared<AWSAuthV4Signer>(ALLOCATION_TAG,
                                             credentialsProvider,
                                             SERVICE_NAME,
                                             Aws::Region::ComputeSignerRegion(clientConfiguration.region)),
            Aws::MakeShared<ACMPCAErrorMarshaller>(ALLOCATION_TAG)),
    m_clientConfiguration(clientConfiguration),
    m_executor(clientConfiguration.executor),
    m_endpointProvider(std::move(endpointProvider))
{
  init(m_clientConfiguration);
}

    /* Legacy constructors due deprecation */
  ACMPCAClient::ACMPCAClient(const Client::ClientConfiguration& clientConfiguration) :
  BASECLASS(clientConfiguration,
            Aws::MakeShared<AWSAuthV4Signer>(ALLOCATION_TAG,
                                             Aws::MakeShared<DefaultAWSCredentialsProviderChain>(ALLOCATION_TAG),
                                             SERVICE_NAME,
                                             Aws::Region::ComputeSignerRegion(clientConfiguration.region)),
            Aws::MakeShared<ACMPCAErrorMarshaller>(ALLOCATION_TAG)),
  m_clientConfiguration(clientConfiguration),
  m_executor(clientConfiguration.executor),
  m_endpointProvider(Aws::MakeShared<ACMPCAEndpointProvider>(ALLOCATION_TAG))
{
  init(m_clientConfiguration);
}

ACMPCAClient::ACMPCAClient(const AWSCredentials& credentials,
                           const Client::ClientConfiguration& clientConfiguration) :
  BASECLASS(clientConfiguration,
            Aws::MakeShared<AWSAuthV4Signer>(ALLOCATION_TAG,
                                             Aws::MakeShared<SimpleAWSCredentialsProvider>(ALLOCATION_TAG, credentials),
                                             SERVICE_NAME,
                                             Aws::Region::ComputeSignerRegion(clientConfiguration.region)),
            Aws::MakeShared<ACMPCAErrorMarshaller>(ALLOCATION_TAG)),
    m_clientConfiguration(clientConfiguration),
    m_executor(clientConfiguration.executor),
    m_endpointProvider(Aws::MakeShared<ACMPCAEndpointProvider>(ALLOCATION_TAG))
{
  init(m_clientConfiguration);
}

ACMPCAClient::ACMPCAClient(const std::shared_ptr<AWSCredentialsProvider>& credentialsProvider,
                           const Client::ClientConfiguration& clientConfiguration) :
  BASECLASS(clientConfiguration,
            Aws::MakeShared<AWSAuthV4Signer>(ALLOCATION_TAG,
                                             credentialsProvider,
                                             SERVICE_NAME,
                                             Aws::Region::ComputeSignerRegion(clientConfiguration.region)),
            Aws::MakeShared<ACMPCAErrorMarshaller>(ALLOCATION_TAG)),
    m_clientConfiguration(clientConfiguration),
    m_executor(clientConfiguration.executor),
    m_endpointProvider(Aws::MakeShared<ACMPCAEndpointProvider>(ALLOCATION_TAG))
{
  init(m_clientConfiguration);
}

    /* End of legacy constructors due deprecation */
ACMPCAClient::~ACMPCAClient()
{
}

std::shared_ptr<ACMPCAEndpointProviderBase>& ACMPCAClient::accessEndpointProvider()
{
  return m_endpointProvider;
}

void ACMPCAClient::init(const ACMPCA::ACMPCAClientConfiguration& config)
{
  AWSClient::SetServiceClientName("ACM PCA");
  AWS_CHECK_PTR(SERVICE_NAME, m_endpointProvider);
  m_endpointProvider->InitBuiltInParameters(config);
}

void ACMPCAClient::OverrideEndpoint(const Aws::String& endpoint)
{
  AWS_CHECK_PTR(SERVICE_NAME, m_endpointProvider);
  m_endpointProvider->OverrideEndpoint(endpoint);
}

CreateCertificateAuthorityOutcome ACMPCAClient::CreateCertificateAuthority(const CreateCertificateAuthorityRequest& request) const
{
  AWS_OPERATION_CHECK_PTR(m_endpointProvider, CreateCertificateAuthority, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE);
  ResolveEndpointOutcome endpointResolutionOutcome = m_endpointProvider->ResolveEndpoint(request.GetEndpointContextParams());
  AWS_OPERATION_CHECK_SUCCESS(endpointResolutionOutcome, CreateCertificateAuthority, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE, endpointResolutionOutcome.GetError().GetMessage());
  return CreateCertificateAuthorityOutcome(MakeRequest(request, endpointResolutionOutcome.GetResult(), Aws::Http::HttpMethod::HTTP_POST, Aws::Auth::SIGV4_SIGNER));
}

CreateCertificateAuthorityOutcomeCallable ACMPCAClient::CreateCertificateAuthorityCallable(const CreateCertificateAuthorityRequest& request) const
{
  std::shared_ptr<CreateCertificateAuthorityRequest> pRequest = request.Clone();
  auto task = Aws::MakeShared< std::packaged_task< CreateCertificateAuthorityOutcome() > >(ALLOCATION_TAG, [this, pRequest](){ return this->CreateCertificateAuthority(*pRequest); } );
  auto packagedFunction = [task]() { (*task)(); };
  m_executor->Submit(packagedFunction);
  return task->get_future();
}
void ACMPCAClient::CreateCertificateAuthorityAsync(const CreateCertificateAuthorityRequest& request, const CreateCertificateAuthorityResponseReceivedHandler& handler, const std::shared_ptr<const Aws::Client::AsyncCallerContext>& context) const
{
  std::shared_ptr<CreateCertificateAuthorityRequest> pRequest = request.Clone();
  m_executor->Submit( [this, pRequest, handler, context]()
    {
      handler(this, *pRequest, CreateCertificateAuthority(*pRequest), context);
    } );
}

CreateCertificateAuthorityAuditReportOutcome ACMPCAClient::CreateCertificateAuthorityAuditReport(const CreateCertificateAuthorityAuditReportRequest& request) const
{
  AWS_OPERATION_CHECK_PTR(m_endpointProvider, CreateCertificateAuthorityAuditReport, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE);
  ResolveEndpointOutcome endpointResolutionOutcome = m_endpointProvider->ResolveEndpoint(request.GetEndpointContextParams());
  AWS_OPERATION_CHECK_SUCCESS(endpointResolutionOutcome, CreateCertificateAuthorityAuditReport, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE, endpointResolutionOutcome.GetError().GetMessage());
  return CreateCertificateAuthorityAuditReportOutcome(MakeRequest(request, endpointResolutionOutcome.GetResult(), Aws::Http::HttpMethod::HTTP_POST, Aws::Auth::SIGV4_SIGNER));
}

CreateCertificateAuthorityAuditReportOutcomeCallable ACMPCAClient::CreateCertificateAuthorityAuditReportCallable(const CreateCertificateAuthorityAuditReportRequest& request) const
{
  std::shared_ptr<CreateCertificateAuthorityAuditReportRequest> pRequest = request.Clone();
  auto task = Aws::MakeShared< std::packaged_task< CreateCertificateAuthorityAuditReportOutcome() > >(ALLOCATION_TAG, [this, pRequest](){ return this->CreateCertificateAuthorityAuditReport(*pRequest); } );
  auto packagedFunction = [task]() { (*task)(); };
  m_executor->Submit(packagedFunction);
  return task->get_future();
}
void ACMPCAClient::CreateCertificateAuthorityAuditReportAsync(const CreateCertificateAuthorityAuditReportRequest& request, const CreateCertificateAuthorityAuditReportResponseReceivedHandler& handler, const std::shared_ptr<const Aws::Client::AsyncCallerContext>& context) const
{
  std::shared_ptr<CreateCertificateAuthorityAuditReportRequest> pRequest = request.Clone();
  m_executor->Submit( [this, pRequest, handler, context]()
    {
      handler(this, *pRequest, CreateCertificateAuthorityAuditReport(*pRequest), context);
    } );
}

CreatePermissionOutcome ACMPCAClient::CreatePermission(const CreatePermissionRequest& request) const
{
  AWS_OPERATION_CHECK_PTR(m_endpointProvider, CreatePermission, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE);
  ResolveEndpointOutcome endpointResolutionOutcome = m_endpointProvider->ResolveEndpoint(request.GetEndpointContextParams());
  AWS_OPERATION_CHECK_SUCCESS(endpointResolutionOutcome, CreatePermission, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE, endpointResolutionOutcome.GetError().GetMessage());
  return CreatePermissionOutcome(MakeRequest(request, endpointResolutionOutcome.GetResult(), Aws::Http::HttpMethod::HTTP_POST, Aws::Auth::SIGV4_SIGNER));
}

CreatePermissionOutcomeCallable ACMPCAClient::CreatePermissionCallable(const CreatePermissionRequest& request) const
{
  std::shared_ptr<CreatePermissionRequest> pRequest = request.Clone();
  auto task = Aws::MakeShared< std::packaged_task< CreatePermissionOutcome() > >(ALLOCATION_TAG, [this, pRequest](){ return this->CreatePermission(*pRequest); } );
  auto packagedFunction = [task]() { (*task)(); };
  m_executor->Submit(packagedFunction);
  return task->get_future();
}
void ACMPCAClient::CreatePermissionAsync(const CreatePermissionRequest& request, const CreatePermissionResponseReceivedHandler& handler, const std::shared_ptr<const Aws::Client::AsyncCallerContext>& context) const
{
  std::shared_ptr<CreatePermissionRequest> pRequest = request.Clone();
  m_executor->Submit( [this, pRequest, handler, context]()
    {
      handler(this, *pRequest, CreatePermission(*pRequest), context);
    } );
}

DeleteCertificateAuthorityOutcome ACMPCAClient::DeleteCertificateAuthority(const DeleteCertificateAuthorityRequest& request) const
{
  AWS_OPERATION_CHECK_PTR(m_endpointProvider, DeleteCertificateAuthority, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE);
  ResolveEndpointOutcome endpointResolutionOutcome = m_endpointProvider->ResolveEndpoint(request.GetEndpointContextParams());
  AWS_OPERATION_CHECK_SUCCESS(endpointResolutionOutcome, DeleteCertificateAuthority, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE, endpointResolutionOutcome.GetError().GetMessage());
  return DeleteCertificateAuthorityOutcome(MakeRequest(request, endpointResolutionOutcome.GetResult(), Aws::Http::HttpMethod::HTTP_POST, Aws::Auth::SIGV4_SIGNER));
}

DeleteCertificateAuthorityOutcomeCallable ACMPCAClient::DeleteCertificateAuthorityCallable(const DeleteCertificateAuthorityRequest& request) const
{
  std::shared_ptr<DeleteCertificateAuthorityRequest> pRequest = request.Clone();
  auto task = Aws::MakeShared< std::packaged_task< DeleteCertificateAuthorityOutcome() > >(ALLOCATION_TAG, [this, pRequest](){ return this->DeleteCertificateAuthority(*pRequest); } );
  auto packagedFunction = [task]() { (*task)(); };
  m_executor->Submit(packagedFunction);
  return task->get_future();
}
void ACMPCAClient::DeleteCertificateAuthorityAsync(const DeleteCertificateAuthorityRequest& request, const DeleteCertificateAuthorityResponseReceivedHandler& handler, const std::shared_ptr<const Aws::Client::AsyncCallerContext>& context) const
{
  std::shared_ptr<DeleteCertificateAuthorityRequest> pRequest = request.Clone();
  m_executor->Submit( [this, pRequest, handler, context]()
    {
      handler(this, *pRequest, DeleteCertificateAuthority(*pRequest), context);
    } );
}

DeletePermissionOutcome ACMPCAClient::DeletePermission(const DeletePermissionRequest& request) const
{
  AWS_OPERATION_CHECK_PTR(m_endpointProvider, DeletePermission, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE);
  ResolveEndpointOutcome endpointResolutionOutcome = m_endpointProvider->ResolveEndpoint(request.GetEndpointContextParams());
  AWS_OPERATION_CHECK_SUCCESS(endpointResolutionOutcome, DeletePermission, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE, endpointResolutionOutcome.GetError().GetMessage());
  return DeletePermissionOutcome(MakeRequest(request, endpointResolutionOutcome.GetResult(), Aws::Http::HttpMethod::HTTP_POST, Aws::Auth::SIGV4_SIGNER));
}

DeletePermissionOutcomeCallable ACMPCAClient::DeletePermissionCallable(const DeletePermissionRequest& request) const
{
  std::shared_ptr<DeletePermissionRequest> pRequest = request.Clone();
  auto task = Aws::MakeShared< std::packaged_task< DeletePermissionOutcome() > >(ALLOCATION_TAG, [this, pRequest](){ return this->DeletePermission(*pRequest); } );
  auto packagedFunction = [task]() { (*task)(); };
  m_executor->Submit(packagedFunction);
  return task->get_future();
}
void ACMPCAClient::DeletePermissionAsync(const DeletePermissionRequest& request, const DeletePermissionResponseReceivedHandler& handler, const std::shared_ptr<const Aws::Client::AsyncCallerContext>& context) const
{
  std::shared_ptr<DeletePermissionRequest> pRequest = request.Clone();
  m_executor->Submit( [this, pRequest, handler, context]()
    {
      handler(this, *pRequest, DeletePermission(*pRequest), context);
    } );
}

DeletePolicyOutcome ACMPCAClient::DeletePolicy(const DeletePolicyRequest& request) const
{
  AWS_OPERATION_CHECK_PTR(m_endpointProvider, DeletePolicy, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE);
  ResolveEndpointOutcome endpointResolutionOutcome = m_endpointProvider->ResolveEndpoint(request.GetEndpointContextParams());
  AWS_OPERATION_CHECK_SUCCESS(endpointResolutionOutcome, DeletePolicy, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE, endpointResolutionOutcome.GetError().GetMessage());
  return DeletePolicyOutcome(MakeRequest(request, endpointResolutionOutcome.GetResult(), Aws::Http::HttpMethod::HTTP_POST, Aws::Auth::SIGV4_SIGNER));
}

DeletePolicyOutcomeCallable ACMPCAClient::DeletePolicyCallable(const DeletePolicyRequest& request) const
{
  std::shared_ptr<DeletePolicyRequest> pRequest = request.Clone();
  auto task = Aws::MakeShared< std::packaged_task< DeletePolicyOutcome() > >(ALLOCATION_TAG, [this, pRequest](){ return this->DeletePolicy(*pRequest); } );
  auto packagedFunction = [task]() { (*task)(); };
  m_executor->Submit(packagedFunction);
  return task->get_future();
}
void ACMPCAClient::DeletePolicyAsync(const DeletePolicyRequest& request, const DeletePolicyResponseReceivedHandler& handler, const std::shared_ptr<const Aws::Client::AsyncCallerContext>& context) const
{
  std::shared_ptr<DeletePolicyRequest> pRequest = request.Clone();
  m_executor->Submit( [this, pRequest, handler, context]()
    {
      handler(this, *pRequest, DeletePolicy(*pRequest), context);
    } );
}

DescribeCertificateAuthorityOutcome ACMPCAClient::DescribeCertificateAuthority(const DescribeCertificateAuthorityRequest& request) const
{
  AWS_OPERATION_CHECK_PTR(m_endpointProvider, DescribeCertificateAuthority, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE);
  ResolveEndpointOutcome endpointResolutionOutcome = m_endpointProvider->ResolveEndpoint(request.GetEndpointContextParams());
  AWS_OPERATION_CHECK_SUCCESS(endpointResolutionOutcome, DescribeCertificateAuthority, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE, endpointResolutionOutcome.GetError().GetMessage());
  return DescribeCertificateAuthorityOutcome(MakeRequest(request, endpointResolutionOutcome.GetResult(), Aws::Http::HttpMethod::HTTP_POST, Aws::Auth::SIGV4_SIGNER));
}

DescribeCertificateAuthorityOutcomeCallable ACMPCAClient::DescribeCertificateAuthorityCallable(const DescribeCertificateAuthorityRequest& request) const
{
  std::shared_ptr<DescribeCertificateAuthorityRequest> pRequest = request.Clone();
  auto task = Aws::MakeShared< std::packaged_task< DescribeCertificateAuthorityOutcome() > >(ALLOCATION_TAG, [this, pRequest](){ return this->DescribeCertificateAuthority(*pRequest); } );
  auto packagedFunction = [task]() { (*task)(); };
  m_executor->Submit(packagedFunction);
  return task->get_future();
}
void ACMPCAClient::DescribeCertificateAuthorityAsync(const DescribeCertificateAuthorityRequest& request, const DescribeCertificateAuthorityResponseReceivedHandler& handler, const std::shared_ptr<const Aws::Client::AsyncCallerContext>& context) const
{
  std::shared_ptr<DescribeCertificateAuthorityRequest> pRequest = request.Clone();
  m_executor->Submit( [this, pRequest, handler, context]()
    {
      handler(this, *pRequest, DescribeCertificateAuthority(*pRequest), context);
    } );
}

DescribeCertificateAuthorityAuditReportOutcome ACMPCAClient::DescribeCertificateAuthorityAuditReport(const DescribeCertificateAuthorityAuditReportRequest& request) const
{
  AWS_OPERATION_CHECK_PTR(m_endpointProvider, DescribeCertificateAuthorityAuditReport, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE);
  ResolveEndpointOutcome endpointResolutionOutcome = m_endpointProvider->ResolveEndpoint(request.GetEndpointContextParams());
  AWS_OPERATION_CHECK_SUCCESS(endpointResolutionOutcome, DescribeCertificateAuthorityAuditReport, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE, endpointResolutionOutcome.GetError().GetMessage());
  return DescribeCertificateAuthorityAuditReportOutcome(MakeRequest(request, endpointResolutionOutcome.GetResult(), Aws::Http::HttpMethod::HTTP_POST, Aws::Auth::SIGV4_SIGNER));
}

DescribeCertificateAuthorityAuditReportOutcomeCallable ACMPCAClient::DescribeCertificateAuthorityAuditReportCallable(const DescribeCertificateAuthorityAuditReportRequest& request) const
{
  std::shared_ptr<DescribeCertificateAuthorityAuditReportRequest> pRequest = request.Clone();
  auto task = Aws::MakeShared< std::packaged_task< DescribeCertificateAuthorityAuditReportOutcome() > >(ALLOCATION_TAG, [this, pRequest](){ return this->DescribeCertificateAuthorityAuditReport(*pRequest); } );
  auto packagedFunction = [task]() { (*task)(); };
  m_executor->Submit(packagedFunction);
  return task->get_future();
}
void ACMPCAClient::DescribeCertificateAuthorityAuditReportAsync(const DescribeCertificateAuthorityAuditReportRequest& request, const DescribeCertificateAuthorityAuditReportResponseReceivedHandler& handler, const std::shared_ptr<const Aws::Client::AsyncCallerContext>& context) const
{
  std::shared_ptr<DescribeCertificateAuthorityAuditReportRequest> pRequest = request.Clone();
  m_executor->Submit( [this, pRequest, handler, context]()
    {
      handler(this, *pRequest, DescribeCertificateAuthorityAuditReport(*pRequest), context);
    } );
}

GetCertificateOutcome ACMPCAClient::GetCertificate(const GetCertificateRequest& request) const
{
  AWS_OPERATION_CHECK_PTR(m_endpointProvider, GetCertificate, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE);
  ResolveEndpointOutcome endpointResolutionOutcome = m_endpointProvider->ResolveEndpoint(request.GetEndpointContextParams());
  AWS_OPERATION_CHECK_SUCCESS(endpointResolutionOutcome, GetCertificate, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE, endpointResolutionOutcome.GetError().GetMessage());
  return GetCertificateOutcome(MakeRequest(request, endpointResolutionOutcome.GetResult(), Aws::Http::HttpMethod::HTTP_POST, Aws::Auth::SIGV4_SIGNER));
}

GetCertificateOutcomeCallable ACMPCAClient::GetCertificateCallable(const GetCertificateRequest& request) const
{
  std::shared_ptr<GetCertificateRequest> pRequest = request.Clone();
  auto task = Aws::MakeShared< std::packaged_task< GetCertificateOutcome() > >(ALLOCATION_TAG, [this, pRequest](){ return this->GetCertificate(*pRequest); } );
  auto packagedFunction = [task]() { (*task)(); };
  m_executor->Submit(packagedFunction);
  return task->get_future();
}
void ACMPCAClient::GetCertificateAsync(const GetCertificateRequest& request, const GetCertificateResponseReceivedHandler& handler, const std::shared_ptr<const Aws::Client::AsyncCallerContext>& context) const
{
  std::shared_ptr<GetCertificateRequest> pRequest = request.Clone();
  m_executor->Submit( [this, pRequest, handler, context]()
    {
      handler(this, *pRequest, GetCertificate(*pRequest), context);
    } );
}

GetCertificateAuthorityCertificateOutcome ACMPCAClient::GetCertificateAuthorityCertificate(const GetCertificateAuthorityCertificateRequest& request) const
{
  AWS_OPERATION_CHECK_PTR(m_endpointProvider, GetCertificateAuthorityCertificate, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE);
  ResolveEndpointOutcome endpointResolutionOutcome = m_endpointProvider->ResolveEndpoint(request.GetEndpointContextParams());
  AWS_OPERATION_CHECK_SUCCESS(endpointResolutionOutcome, GetCertificateAuthorityCertificate, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE, endpointResolutionOutcome.GetError().GetMessage());
  return GetCertificateAuthorityCertificateOutcome(MakeRequest(request, endpointResolutionOutcome.GetResult(), Aws::Http::HttpMethod::HTTP_POST, Aws::Auth::SIGV4_SIGNER));
}

GetCertificateAuthorityCertificateOutcomeCallable ACMPCAClient::GetCertificateAuthorityCertificateCallable(const GetCertificateAuthorityCertificateRequest& request) const
{
  std::shared_ptr<GetCertificateAuthorityCertificateRequest> pRequest = request.Clone();
  auto task = Aws::MakeShared< std::packaged_task< GetCertificateAuthorityCertificateOutcome() > >(ALLOCATION_TAG, [this, pRequest](){ return this->GetCertificateAuthorityCertificate(*pRequest); } );
  auto packagedFunction = [task]() { (*task)(); };
  m_executor->Submit(packagedFunction);
  return task->get_future();
}
void ACMPCAClient::GetCertificateAuthorityCertificateAsync(const GetCertificateAuthorityCertificateRequest& request, const GetCertificateAuthorityCertificateResponseReceivedHandler& handler, const std::shared_ptr<const Aws::Client::AsyncCallerContext>& context) const
{
  std::shared_ptr<GetCertificateAuthorityCertificateRequest> pRequest = request.Clone();
  m_executor->Submit( [this, pRequest, handler, context]()
    {
      handler(this, *pRequest, GetCertificateAuthorityCertificate(*pRequest), context);
    } );
}

GetCertificateAuthorityCsrOutcome ACMPCAClient::GetCertificateAuthorityCsr(const GetCertificateAuthorityCsrRequest& request) const
{
  AWS_OPERATION_CHECK_PTR(m_endpointProvider, GetCertificateAuthorityCsr, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE);
  ResolveEndpointOutcome endpointResolutionOutcome = m_endpointProvider->ResolveEndpoint(request.GetEndpointContextParams());
  AWS_OPERATION_CHECK_SUCCESS(endpointResolutionOutcome, GetCertificateAuthorityCsr, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE, endpointResolutionOutcome.GetError().GetMessage());
  return GetCertificateAuthorityCsrOutcome(MakeRequest(request, endpointResolutionOutcome.GetResult(), Aws::Http::HttpMethod::HTTP_POST, Aws::Auth::SIGV4_SIGNER));
}

GetCertificateAuthorityCsrOutcomeCallable ACMPCAClient::GetCertificateAuthorityCsrCallable(const GetCertificateAuthorityCsrRequest& request) const
{
  std::shared_ptr<GetCertificateAuthorityCsrRequest> pRequest = request.Clone();
  auto task = Aws::MakeShared< std::packaged_task< GetCertificateAuthorityCsrOutcome() > >(ALLOCATION_TAG, [this, pRequest](){ return this->GetCertificateAuthorityCsr(*pRequest); } );
  auto packagedFunction = [task]() { (*task)(); };
  m_executor->Submit(packagedFunction);
  return task->get_future();
}
void ACMPCAClient::GetCertificateAuthorityCsrAsync(const GetCertificateAuthorityCsrRequest& request, const GetCertificateAuthorityCsrResponseReceivedHandler& handler, const std::shared_ptr<const Aws::Client::AsyncCallerContext>& context) const
{
  std::shared_ptr<GetCertificateAuthorityCsrRequest> pRequest = request.Clone();
  m_executor->Submit( [this, pRequest, handler, context]()
    {
      handler(this, *pRequest, GetCertificateAuthorityCsr(*pRequest), context);
    } );
}

GetPolicyOutcome ACMPCAClient::GetPolicy(const GetPolicyRequest& request) const
{
  AWS_OPERATION_CHECK_PTR(m_endpointProvider, GetPolicy, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE);
  ResolveEndpointOutcome endpointResolutionOutcome = m_endpointProvider->ResolveEndpoint(request.GetEndpointContextParams());
  AWS_OPERATION_CHECK_SUCCESS(endpointResolutionOutcome, GetPolicy, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE, endpointResolutionOutcome.GetError().GetMessage());
  return GetPolicyOutcome(MakeRequest(request, endpointResolutionOutcome.GetResult(), Aws::Http::HttpMethod::HTTP_POST, Aws::Auth::SIGV4_SIGNER));
}

GetPolicyOutcomeCallable ACMPCAClient::GetPolicyCallable(const GetPolicyRequest& request) const
{
  std::shared_ptr<GetPolicyRequest> pRequest = request.Clone();
  auto task = Aws::MakeShared< std::packaged_task< GetPolicyOutcome() > >(ALLOCATION_TAG, [this, pRequest](){ return this->GetPolicy(*pRequest); } );
  auto packagedFunction = [task]() { (*task)(); };
  m_executor->Submit(packagedFunction);
  return task->get_future();
}
void ACMPCAClient::GetPolicyAsync(const GetPolicyRequest& request, const GetPolicyResponseReceivedHandler& handler, const std::shared_ptr<const Aws::Client::AsyncCallerContext>& context) const
{
  std::shared_ptr<GetPolicyRequest> pRequest = request.Clone();
  m_executor->Submit( [this, pRequest, handler, context]()
    {
      handler(this, *pRequest, GetPolicy(*pRequest), context);
    } );
}

ImportCertificateAuthorityCertificateOutcome ACMPCAClient::ImportCertificateAuthorityCertificate(const ImportCertificateAuthorityCertificateRequest& request) const
{
  AWS_OPERATION_CHECK_PTR(m_endpointProvider, ImportCertificateAuthorityCertificate, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE);
  ResolveEndpointOutcome endpointResolutionOutcome = m_endpointProvider->ResolveEndpoint(request.GetEndpointContextParams());
  AWS_OPERATION_CHECK_SUCCESS(endpointResolutionOutcome, ImportCertificateAuthorityCertificate, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE, endpointResolutionOutcome.GetError().GetMessage());
  return ImportCertificateAuthorityCertificateOutcome(MakeRequest(request, endpointResolutionOutcome.GetResult(), Aws::Http::HttpMethod::HTTP_POST, Aws::Auth::SIGV4_SIGNER));
}

ImportCertificateAuthorityCertificateOutcomeCallable ACMPCAClient::ImportCertificateAuthorityCertificateCallable(const ImportCertificateAuthorityCertificateRequest& request) const
{
  std::shared_ptr<ImportCertificateAuthorityCertificateRequest> pRequest = request.Clone();
  auto task = Aws::MakeShared< std::packaged_task< ImportCertificateAuthorityCertificateOutcome() > >(ALLOCATION_TAG, [this, pRequest](){ return this->ImportCertificateAuthorityCertificate(*pRequest); } );
  auto packagedFunction = [task]() { (*task)(); };
  m_executor->Submit(packagedFunction);
  return task->get_future();
}
void ACMPCAClient::ImportCertificateAuthorityCertificateAsync(const ImportCertificateAuthorityCertificateRequest& request, const ImportCertificateAuthorityCertificateResponseReceivedHandler& handler, const std::shared_ptr<const Aws::Client::AsyncCallerContext>& context) const
{
  std::shared_ptr<ImportCertificateAuthorityCertificateRequest> pRequest = request.Clone();
  m_executor->Submit( [this, pRequest, handler, context]()
    {
      handler(this, *pRequest, ImportCertificateAuthorityCertificate(*pRequest), context);
    } );
}

IssueCertificateOutcome ACMPCAClient::IssueCertificate(const IssueCertificateRequest& request) const
{
  AWS_OPERATION_CHECK_PTR(m_endpointProvider, IssueCertificate, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE);
  ResolveEndpointOutcome endpointResolutionOutcome = m_endpointProvider->ResolveEndpoint(request.GetEndpointContextParams());
  AWS_OPERATION_CHECK_SUCCESS(endpointResolutionOutcome, IssueCertificate, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE, endpointResolutionOutcome.GetError().GetMessage());
  return IssueCertificateOutcome(MakeRequest(request, endpointResolutionOutcome.GetResult(), Aws::Http::HttpMethod::HTTP_POST, Aws::Auth::SIGV4_SIGNER));
}

IssueCertificateOutcomeCallable ACMPCAClient::IssueCertificateCallable(const IssueCertificateRequest& request) const
{
  std::shared_ptr<IssueCertificateRequest> pRequest = request.Clone();
  auto task = Aws::MakeShared< std::packaged_task< IssueCertificateOutcome() > >(ALLOCATION_TAG, [this, pRequest](){ return this->IssueCertificate(*pRequest); } );
  auto packagedFunction = [task]() { (*task)(); };
  m_executor->Submit(packagedFunction);
  return task->get_future();
}
void ACMPCAClient::IssueCertificateAsync(const IssueCertificateRequest& request, const IssueCertificateResponseReceivedHandler& handler, const std::shared_ptr<const Aws::Client::AsyncCallerContext>& context) const
{
  std::shared_ptr<IssueCertificateRequest> pRequest = request.Clone();
  m_executor->Submit( [this, pRequest, handler, context]()
    {
      handler(this, *pRequest, IssueCertificate(*pRequest), context);
    } );
}

ListCertificateAuthoritiesOutcome ACMPCAClient::ListCertificateAuthorities(const ListCertificateAuthoritiesRequest& request) const
{
  AWS_OPERATION_CHECK_PTR(m_endpointProvider, ListCertificateAuthorities, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE);
  ResolveEndpointOutcome endpointResolutionOutcome = m_endpointProvider->ResolveEndpoint(request.GetEndpointContextParams());
  AWS_OPERATION_CHECK_SUCCESS(endpointResolutionOutcome, ListCertificateAuthorities, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE, endpointResolutionOutcome.GetError().GetMessage());
  return ListCertificateAuthoritiesOutcome(MakeRequest(request, endpointResolutionOutcome.GetResult(), Aws::Http::HttpMethod::HTTP_POST, Aws::Auth::SIGV4_SIGNER));
}

ListCertificateAuthoritiesOutcomeCallable ACMPCAClient::ListCertificateAuthoritiesCallable(const ListCertificateAuthoritiesRequest& request) const
{
  std::shared_ptr<ListCertificateAuthoritiesRequest> pRequest = request.Clone();
  auto task = Aws::MakeShared< std::packaged_task< ListCertificateAuthoritiesOutcome() > >(ALLOCATION_TAG, [this, pRequest](){ return this->ListCertificateAuthorities(*pRequest); } );
  auto packagedFunction = [task]() { (*task)(); };
  m_executor->Submit(packagedFunction);
  return task->get_future();
}
void ACMPCAClient::ListCertificateAuthoritiesAsync(const ListCertificateAuthoritiesRequest& request, const ListCertificateAuthoritiesResponseReceivedHandler& handler, const std::shared_ptr<const Aws::Client::AsyncCallerContext>& context) const
{
  std::shared_ptr<ListCertificateAuthoritiesRequest> pRequest = request.Clone();
  m_executor->Submit( [this, pRequest, handler, context]()
    {
      handler(this, *pRequest, ListCertificateAuthorities(*pRequest), context);
    } );
}

ListPermissionsOutcome ACMPCAClient::ListPermissions(const ListPermissionsRequest& request) const
{
  AWS_OPERATION_CHECK_PTR(m_endpointProvider, ListPermissions, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE);
  ResolveEndpointOutcome endpointResolutionOutcome = m_endpointProvider->ResolveEndpoint(request.GetEndpointContextParams());
  AWS_OPERATION_CHECK_SUCCESS(endpointResolutionOutcome, ListPermissions, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE, endpointResolutionOutcome.GetError().GetMessage());
  return ListPermissionsOutcome(MakeRequest(request, endpointResolutionOutcome.GetResult(), Aws::Http::HttpMethod::HTTP_POST, Aws::Auth::SIGV4_SIGNER));
}

ListPermissionsOutcomeCallable ACMPCAClient::ListPermissionsCallable(const ListPermissionsRequest& request) const
{
  std::shared_ptr<ListPermissionsRequest> pRequest = request.Clone();
  auto task = Aws::MakeShared< std::packaged_task< ListPermissionsOutcome() > >(ALLOCATION_TAG, [this, pRequest](){ return this->ListPermissions(*pRequest); } );
  auto packagedFunction = [task]() { (*task)(); };
  m_executor->Submit(packagedFunction);
  return task->get_future();
}
void ACMPCAClient::ListPermissionsAsync(const ListPermissionsRequest& request, const ListPermissionsResponseReceivedHandler& handler, const std::shared_ptr<const Aws::Client::AsyncCallerContext>& context) const
{
  std::shared_ptr<ListPermissionsRequest> pRequest = request.Clone();
  m_executor->Submit( [this, pRequest, handler, context]()
    {
      handler(this, *pRequest, ListPermissions(*pRequest), context);
    } );
}

ListTagsOutcome ACMPCAClient::ListTags(const ListTagsRequest& request) const
{
  AWS_OPERATION_CHECK_PTR(m_endpointProvider, ListTags, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE);
  ResolveEndpointOutcome endpointResolutionOutcome = m_endpointProvider->ResolveEndpoint(request.GetEndpointContextParams());
  AWS_OPERATION_CHECK_SUCCESS(endpointResolutionOutcome, ListTags, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE, endpointResolutionOutcome.GetError().GetMessage());
  return ListTagsOutcome(MakeRequest(request, endpointResolutionOutcome.GetResult(), Aws::Http::HttpMethod::HTTP_POST, Aws::Auth::SIGV4_SIGNER));
}

ListTagsOutcomeCallable ACMPCAClient::ListTagsCallable(const ListTagsRequest& request) const
{
  std::shared_ptr<ListTagsRequest> pRequest = request.Clone();
  auto task = Aws::MakeShared< std::packaged_task< ListTagsOutcome() > >(ALLOCATION_TAG, [this, pRequest](){ return this->ListTags(*pRequest); } );
  auto packagedFunction = [task]() { (*task)(); };
  m_executor->Submit(packagedFunction);
  return task->get_future();
}
void ACMPCAClient::ListTagsAsync(const ListTagsRequest& request, const ListTagsResponseReceivedHandler& handler, const std::shared_ptr<const Aws::Client::AsyncCallerContext>& context) const
{
  std::shared_ptr<ListTagsRequest> pRequest = request.Clone();
  m_executor->Submit( [this, pRequest, handler, context]()
    {
      handler(this, *pRequest, ListTags(*pRequest), context);
    } );
}

PutPolicyOutcome ACMPCAClient::PutPolicy(const PutPolicyRequest& request) const
{
  AWS_OPERATION_CHECK_PTR(m_endpointProvider, PutPolicy, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE);
  ResolveEndpointOutcome endpointResolutionOutcome = m_endpointProvider->ResolveEndpoint(request.GetEndpointContextParams());
  AWS_OPERATION_CHECK_SUCCESS(endpointResolutionOutcome, PutPolicy, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE, endpointResolutionOutcome.GetError().GetMessage());
  return PutPolicyOutcome(MakeRequest(request, endpointResolutionOutcome.GetResult(), Aws::Http::HttpMethod::HTTP_POST, Aws::Auth::SIGV4_SIGNER));
}

PutPolicyOutcomeCallable ACMPCAClient::PutPolicyCallable(const PutPolicyRequest& request) const
{
  std::shared_ptr<PutPolicyRequest> pRequest = request.Clone();
  auto task = Aws::MakeShared< std::packaged_task< PutPolicyOutcome() > >(ALLOCATION_TAG, [this, pRequest](){ return this->PutPolicy(*pRequest); } );
  auto packagedFunction = [task]() { (*task)(); };
  m_executor->Submit(packagedFunction);
  return task->get_future();
}
void ACMPCAClient::PutPolicyAsync(const PutPolicyRequest& request, const PutPolicyResponseReceivedHandler& handler, const std::shared_ptr<const Aws::Client::AsyncCallerContext>& context) const
{
  std::shared_ptr<PutPolicyRequest> pRequest = request.Clone();
  m_executor->Submit( [this, pRequest, handler, context]()
    {
      handler(this, *pRequest, PutPolicy(*pRequest), context);
    } );
}

RestoreCertificateAuthorityOutcome ACMPCAClient::RestoreCertificateAuthority(const RestoreCertificateAuthorityRequest& request) const
{
  AWS_OPERATION_CHECK_PTR(m_endpointProvider, RestoreCertificateAuthority, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE);
  ResolveEndpointOutcome endpointResolutionOutcome = m_endpointProvider->ResolveEndpoint(request.GetEndpointContextParams());
  AWS_OPERATION_CHECK_SUCCESS(endpointResolutionOutcome, RestoreCertificateAuthority, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE, endpointResolutionOutcome.GetError().GetMessage());
  return RestoreCertificateAuthorityOutcome(MakeRequest(request, endpointResolutionOutcome.GetResult(), Aws::Http::HttpMethod::HTTP_POST, Aws::Auth::SIGV4_SIGNER));
}

RestoreCertificateAuthorityOutcomeCallable ACMPCAClient::RestoreCertificateAuthorityCallable(const RestoreCertificateAuthorityRequest& request) const
{
  std::shared_ptr<RestoreCertificateAuthorityRequest> pRequest = request.Clone();
  auto task = Aws::MakeShared< std::packaged_task< RestoreCertificateAuthorityOutcome() > >(ALLOCATION_TAG, [this, pRequest](){ return this->RestoreCertificateAuthority(*pRequest); } );
  auto packagedFunction = [task]() { (*task)(); };
  m_executor->Submit(packagedFunction);
  return task->get_future();
}
void ACMPCAClient::RestoreCertificateAuthorityAsync(const RestoreCertificateAuthorityRequest& request, const RestoreCertificateAuthorityResponseReceivedHandler& handler, const std::shared_ptr<const Aws::Client::AsyncCallerContext>& context) const
{
  std::shared_ptr<RestoreCertificateAuthorityRequest> pRequest = request.Clone();
  m_executor->Submit( [this, pRequest, handler, context]()
    {
      handler(this, *pRequest, RestoreCertificateAuthority(*pRequest), context);
    } );
}

RevokeCertificateOutcome ACMPCAClient::RevokeCertificate(const RevokeCertificateRequest& request) const
{
  AWS_OPERATION_CHECK_PTR(m_endpointProvider, RevokeCertificate, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE);
  ResolveEndpointOutcome endpointResolutionOutcome = m_endpointProvider->ResolveEndpoint(request.GetEndpointContextParams());
  AWS_OPERATION_CHECK_SUCCESS(endpointResolutionOutcome, RevokeCertificate, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE, endpointResolutionOutcome.GetError().GetMessage());
  return RevokeCertificateOutcome(MakeRequest(request, endpointResolutionOutcome.GetResult(), Aws::Http::HttpMethod::HTTP_POST, Aws::Auth::SIGV4_SIGNER));
}

RevokeCertificateOutcomeCallable ACMPCAClient::RevokeCertificateCallable(const RevokeCertificateRequest& request) const
{
  std::shared_ptr<RevokeCertificateRequest> pRequest = request.Clone();
  auto task = Aws::MakeShared< std::packaged_task< RevokeCertificateOutcome() > >(ALLOCATION_TAG, [this, pRequest](){ return this->RevokeCertificate(*pRequest); } );
  auto packagedFunction = [task]() { (*task)(); };
  m_executor->Submit(packagedFunction);
  return task->get_future();
}
void ACMPCAClient::RevokeCertificateAsync(const RevokeCertificateRequest& request, const RevokeCertificateResponseReceivedHandler& handler, const std::shared_ptr<const Aws::Client::AsyncCallerContext>& context) const
{
  std::shared_ptr<RevokeCertificateRequest> pRequest = request.Clone();
  m_executor->Submit( [this, pRequest, handler, context]()
    {
      handler(this, *pRequest, RevokeCertificate(*pRequest), context);
    } );
}

TagCertificateAuthorityOutcome ACMPCAClient::TagCertificateAuthority(const TagCertificateAuthorityRequest& request) const
{
  AWS_OPERATION_CHECK_PTR(m_endpointProvider, TagCertificateAuthority, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE);
  ResolveEndpointOutcome endpointResolutionOutcome = m_endpointProvider->ResolveEndpoint(request.GetEndpointContextParams());
  AWS_OPERATION_CHECK_SUCCESS(endpointResolutionOutcome, TagCertificateAuthority, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE, endpointResolutionOutcome.GetError().GetMessage());
  return TagCertificateAuthorityOutcome(MakeRequest(request, endpointResolutionOutcome.GetResult(), Aws::Http::HttpMethod::HTTP_POST, Aws::Auth::SIGV4_SIGNER));
}

TagCertificateAuthorityOutcomeCallable ACMPCAClient::TagCertificateAuthorityCallable(const TagCertificateAuthorityRequest& request) const
{
  std::shared_ptr<TagCertificateAuthorityRequest> pRequest = request.Clone();
  auto task = Aws::MakeShared< std::packaged_task< TagCertificateAuthorityOutcome() > >(ALLOCATION_TAG, [this, pRequest](){ return this->TagCertificateAuthority(*pRequest); } );
  auto packagedFunction = [task]() { (*task)(); };
  m_executor->Submit(packagedFunction);
  return task->get_future();
}
void ACMPCAClient::TagCertificateAuthorityAsync(const TagCertificateAuthorityRequest& request, const TagCertificateAuthorityResponseReceivedHandler& handler, const std::shared_ptr<const Aws::Client::AsyncCallerContext>& context) const
{
  std::shared_ptr<TagCertificateAuthorityRequest> pRequest = request.Clone();
  m_executor->Submit( [this, pRequest, handler, context]()
    {
      handler(this, *pRequest, TagCertificateAuthority(*pRequest), context);
    } );
}

UntagCertificateAuthorityOutcome ACMPCAClient::UntagCertificateAuthority(const UntagCertificateAuthorityRequest& request) const
{
  AWS_OPERATION_CHECK_PTR(m_endpointProvider, UntagCertificateAuthority, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE);
  ResolveEndpointOutcome endpointResolutionOutcome = m_endpointProvider->ResolveEndpoint(request.GetEndpointContextParams());
  AWS_OPERATION_CHECK_SUCCESS(endpointResolutionOutcome, UntagCertificateAuthority, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE, endpointResolutionOutcome.GetError().GetMessage());
  return UntagCertificateAuthorityOutcome(MakeRequest(request, endpointResolutionOutcome.GetResult(), Aws::Http::HttpMethod::HTTP_POST, Aws::Auth::SIGV4_SIGNER));
}

UntagCertificateAuthorityOutcomeCallable ACMPCAClient::UntagCertificateAuthorityCallable(const UntagCertificateAuthorityRequest& request) const
{
  std::shared_ptr<UntagCertificateAuthorityRequest> pRequest = request.Clone();
  auto task = Aws::MakeShared< std::packaged_task< UntagCertificateAuthorityOutcome() > >(ALLOCATION_TAG, [this, pRequest](){ return this->UntagCertificateAuthority(*pRequest); } );
  auto packagedFunction = [task]() { (*task)(); };
  m_executor->Submit(packagedFunction);
  return task->get_future();
}
void ACMPCAClient::UntagCertificateAuthorityAsync(const UntagCertificateAuthorityRequest& request, const UntagCertificateAuthorityResponseReceivedHandler& handler, const std::shared_ptr<const Aws::Client::AsyncCallerContext>& context) const
{
  std::shared_ptr<UntagCertificateAuthorityRequest> pRequest = request.Clone();
  m_executor->Submit( [this, pRequest, handler, context]()
    {
      handler(this, *pRequest, UntagCertificateAuthority(*pRequest), context);
    } );
}

UpdateCertificateAuthorityOutcome ACMPCAClient::UpdateCertificateAuthority(const UpdateCertificateAuthorityRequest& request) const
{
  AWS_OPERATION_CHECK_PTR(m_endpointProvider, UpdateCertificateAuthority, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE);
  ResolveEndpointOutcome endpointResolutionOutcome = m_endpointProvider->ResolveEndpoint(request.GetEndpointContextParams());
  AWS_OPERATION_CHECK_SUCCESS(endpointResolutionOutcome, UpdateCertificateAuthority, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE, endpointResolutionOutcome.GetError().GetMessage());
  return UpdateCertificateAuthorityOutcome(MakeRequest(request, endpointResolutionOutcome.GetResult(), Aws::Http::HttpMethod::HTTP_POST, Aws::Auth::SIGV4_SIGNER));
}

UpdateCertificateAuthorityOutcomeCallable ACMPCAClient::UpdateCertificateAuthorityCallable(const UpdateCertificateAuthorityRequest& request) const
{
  std::shared_ptr<UpdateCertificateAuthorityRequest> pRequest = request.Clone();
  auto task = Aws::MakeShared< std::packaged_task< UpdateCertificateAuthorityOutcome() > >(ALLOCATION_TAG, [this, pRequest](){ return this->UpdateCertificateAuthority(*pRequest); } );
  auto packagedFunction = [task]() { (*task)(); };
  m_executor->Submit(packagedFunction);
  return task->get_future();
}
void ACMPCAClient::UpdateCertificateAuthorityAsync(const UpdateCertificateAuthorityRequest& request, const UpdateCertificateAuthorityResponseReceivedHandler& handler, const std::shared_ptr<const Aws::Client::AsyncCallerContext>& context) const
{
  std::shared_ptr<UpdateCertificateAuthorityRequest> pRequest = request.Clone();
  m_executor->Submit( [this, pRequest, handler, context]()
    {
      handler(this, *pRequest, UpdateCertificateAuthority(*pRequest), context);
    } );
}

