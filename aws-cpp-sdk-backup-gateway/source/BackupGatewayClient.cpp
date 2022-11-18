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

#include <aws/backup-gateway/BackupGatewayClient.h>
#include <aws/backup-gateway/BackupGatewayErrorMarshaller.h>
#include <aws/backup-gateway/BackupGatewayEndpointProvider.h>
#include <aws/backup-gateway/model/AssociateGatewayToServerRequest.h>
#include <aws/backup-gateway/model/CreateGatewayRequest.h>
#include <aws/backup-gateway/model/DeleteGatewayRequest.h>
#include <aws/backup-gateway/model/DeleteHypervisorRequest.h>
#include <aws/backup-gateway/model/DisassociateGatewayFromServerRequest.h>
#include <aws/backup-gateway/model/GetGatewayRequest.h>
#include <aws/backup-gateway/model/GetVirtualMachineRequest.h>
#include <aws/backup-gateway/model/ImportHypervisorConfigurationRequest.h>
#include <aws/backup-gateway/model/ListGatewaysRequest.h>
#include <aws/backup-gateway/model/ListHypervisorsRequest.h>
#include <aws/backup-gateway/model/ListTagsForResourceRequest.h>
#include <aws/backup-gateway/model/ListVirtualMachinesRequest.h>
#include <aws/backup-gateway/model/PutMaintenanceStartTimeRequest.h>
#include <aws/backup-gateway/model/TagResourceRequest.h>
#include <aws/backup-gateway/model/TestHypervisorConfigurationRequest.h>
#include <aws/backup-gateway/model/UntagResourceRequest.h>
#include <aws/backup-gateway/model/UpdateGatewayInformationRequest.h>
#include <aws/backup-gateway/model/UpdateGatewaySoftwareNowRequest.h>
#include <aws/backup-gateway/model/UpdateHypervisorRequest.h>

using namespace Aws;
using namespace Aws::Auth;
using namespace Aws::Client;
using namespace Aws::BackupGateway;
using namespace Aws::BackupGateway::Model;
using namespace Aws::Http;
using namespace Aws::Utils::Json;
using ResolveEndpointOutcome = Aws::Endpoint::ResolveEndpointOutcome;

const char* BackupGatewayClient::SERVICE_NAME = "backup-gateway";
const char* BackupGatewayClient::ALLOCATION_TAG = "BackupGatewayClient";

BackupGatewayClient::BackupGatewayClient(const BackupGateway::BackupGatewayClientConfiguration& clientConfiguration,
                                         std::shared_ptr<BackupGatewayEndpointProviderBase> endpointProvider) :
  BASECLASS(clientConfiguration,
            Aws::MakeShared<AWSAuthV4Signer>(ALLOCATION_TAG,
                                             Aws::MakeShared<DefaultAWSCredentialsProviderChain>(ALLOCATION_TAG),
                                             SERVICE_NAME,
                                             Aws::Region::ComputeSignerRegion(clientConfiguration.region)),
            Aws::MakeShared<BackupGatewayErrorMarshaller>(ALLOCATION_TAG)),
  m_clientConfiguration(clientConfiguration),
  m_executor(clientConfiguration.executor),
  m_endpointProvider(std::move(endpointProvider))
{
  init(m_clientConfiguration);
}

BackupGatewayClient::BackupGatewayClient(const AWSCredentials& credentials,
                                         std::shared_ptr<BackupGatewayEndpointProviderBase> endpointProvider,
                                         const BackupGateway::BackupGatewayClientConfiguration& clientConfiguration) :
  BASECLASS(clientConfiguration,
            Aws::MakeShared<AWSAuthV4Signer>(ALLOCATION_TAG,
                                             Aws::MakeShared<SimpleAWSCredentialsProvider>(ALLOCATION_TAG, credentials),
                                             SERVICE_NAME,
                                             Aws::Region::ComputeSignerRegion(clientConfiguration.region)),
            Aws::MakeShared<BackupGatewayErrorMarshaller>(ALLOCATION_TAG)),
    m_clientConfiguration(clientConfiguration),
    m_executor(clientConfiguration.executor),
    m_endpointProvider(std::move(endpointProvider))
{
  init(m_clientConfiguration);
}

BackupGatewayClient::BackupGatewayClient(const std::shared_ptr<AWSCredentialsProvider>& credentialsProvider,
                                         std::shared_ptr<BackupGatewayEndpointProviderBase> endpointProvider,
                                         const BackupGateway::BackupGatewayClientConfiguration& clientConfiguration) :
  BASECLASS(clientConfiguration,
            Aws::MakeShared<AWSAuthV4Signer>(ALLOCATION_TAG,
                                             credentialsProvider,
                                             SERVICE_NAME,
                                             Aws::Region::ComputeSignerRegion(clientConfiguration.region)),
            Aws::MakeShared<BackupGatewayErrorMarshaller>(ALLOCATION_TAG)),
    m_clientConfiguration(clientConfiguration),
    m_executor(clientConfiguration.executor),
    m_endpointProvider(std::move(endpointProvider))
{
  init(m_clientConfiguration);
}

    /* Legacy constructors due deprecation */
  BackupGatewayClient::BackupGatewayClient(const Client::ClientConfiguration& clientConfiguration) :
  BASECLASS(clientConfiguration,
            Aws::MakeShared<AWSAuthV4Signer>(ALLOCATION_TAG,
                                             Aws::MakeShared<DefaultAWSCredentialsProviderChain>(ALLOCATION_TAG),
                                             SERVICE_NAME,
                                             Aws::Region::ComputeSignerRegion(clientConfiguration.region)),
            Aws::MakeShared<BackupGatewayErrorMarshaller>(ALLOCATION_TAG)),
  m_clientConfiguration(clientConfiguration),
  m_executor(clientConfiguration.executor),
  m_endpointProvider(Aws::MakeShared<BackupGatewayEndpointProvider>(ALLOCATION_TAG))
{
  init(m_clientConfiguration);
}

BackupGatewayClient::BackupGatewayClient(const AWSCredentials& credentials,
                                         const Client::ClientConfiguration& clientConfiguration) :
  BASECLASS(clientConfiguration,
            Aws::MakeShared<AWSAuthV4Signer>(ALLOCATION_TAG,
                                             Aws::MakeShared<SimpleAWSCredentialsProvider>(ALLOCATION_TAG, credentials),
                                             SERVICE_NAME,
                                             Aws::Region::ComputeSignerRegion(clientConfiguration.region)),
            Aws::MakeShared<BackupGatewayErrorMarshaller>(ALLOCATION_TAG)),
    m_clientConfiguration(clientConfiguration),
    m_executor(clientConfiguration.executor),
    m_endpointProvider(Aws::MakeShared<BackupGatewayEndpointProvider>(ALLOCATION_TAG))
{
  init(m_clientConfiguration);
}

BackupGatewayClient::BackupGatewayClient(const std::shared_ptr<AWSCredentialsProvider>& credentialsProvider,
                                         const Client::ClientConfiguration& clientConfiguration) :
  BASECLASS(clientConfiguration,
            Aws::MakeShared<AWSAuthV4Signer>(ALLOCATION_TAG,
                                             credentialsProvider,
                                             SERVICE_NAME,
                                             Aws::Region::ComputeSignerRegion(clientConfiguration.region)),
            Aws::MakeShared<BackupGatewayErrorMarshaller>(ALLOCATION_TAG)),
    m_clientConfiguration(clientConfiguration),
    m_executor(clientConfiguration.executor),
    m_endpointProvider(Aws::MakeShared<BackupGatewayEndpointProvider>(ALLOCATION_TAG))
{
  init(m_clientConfiguration);
}

    /* End of legacy constructors due deprecation */
BackupGatewayClient::~BackupGatewayClient()
{
}

std::shared_ptr<BackupGatewayEndpointProviderBase>& BackupGatewayClient::accessEndpointProvider()
{
  return m_endpointProvider;
}

void BackupGatewayClient::init(const BackupGateway::BackupGatewayClientConfiguration& config)
{
  AWSClient::SetServiceClientName("Backup Gateway");
  AWS_CHECK_PTR(SERVICE_NAME, m_endpointProvider);
  m_endpointProvider->InitBuiltInParameters(config);
}

void BackupGatewayClient::OverrideEndpoint(const Aws::String& endpoint)
{
  AWS_CHECK_PTR(SERVICE_NAME, m_endpointProvider);
  m_endpointProvider->OverrideEndpoint(endpoint);
}

AssociateGatewayToServerOutcome BackupGatewayClient::AssociateGatewayToServer(const AssociateGatewayToServerRequest& request) const
{
  AWS_OPERATION_CHECK_PTR(m_endpointProvider, AssociateGatewayToServer, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE);
  ResolveEndpointOutcome endpointResolutionOutcome = m_endpointProvider->ResolveEndpoint(request.GetEndpointContextParams());
  AWS_OPERATION_CHECK_SUCCESS(endpointResolutionOutcome, AssociateGatewayToServer, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE, endpointResolutionOutcome.GetError().GetMessage());
  return AssociateGatewayToServerOutcome(MakeRequest(request, endpointResolutionOutcome.GetResult(), Aws::Http::HttpMethod::HTTP_POST, Aws::Auth::SIGV4_SIGNER));
}

AssociateGatewayToServerOutcomeCallable BackupGatewayClient::AssociateGatewayToServerCallable(const AssociateGatewayToServerRequest& request) const
{
  AWS_MAKE_CALLABLE_OPERATION(AssociateGatewayToServer, request, m_executor.get());
}

void BackupGatewayClient::AssociateGatewayToServerAsync(const AssociateGatewayToServerRequest& request, const AssociateGatewayToServerResponseReceivedHandler& handler, const std::shared_ptr<const Aws::Client::AsyncCallerContext>& context) const
{
  AWS_MAKE_ASYNC_OPERATION(AssociateGatewayToServer, request, handler, context, m_executor.get());
}

CreateGatewayOutcome BackupGatewayClient::CreateGateway(const CreateGatewayRequest& request) const
{
  AWS_OPERATION_CHECK_PTR(m_endpointProvider, CreateGateway, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE);
  ResolveEndpointOutcome endpointResolutionOutcome = m_endpointProvider->ResolveEndpoint(request.GetEndpointContextParams());
  AWS_OPERATION_CHECK_SUCCESS(endpointResolutionOutcome, CreateGateway, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE, endpointResolutionOutcome.GetError().GetMessage());
  return CreateGatewayOutcome(MakeRequest(request, endpointResolutionOutcome.GetResult(), Aws::Http::HttpMethod::HTTP_POST, Aws::Auth::SIGV4_SIGNER));
}

CreateGatewayOutcomeCallable BackupGatewayClient::CreateGatewayCallable(const CreateGatewayRequest& request) const
{
  AWS_MAKE_CALLABLE_OPERATION(CreateGateway, request, m_executor.get());
}

void BackupGatewayClient::CreateGatewayAsync(const CreateGatewayRequest& request, const CreateGatewayResponseReceivedHandler& handler, const std::shared_ptr<const Aws::Client::AsyncCallerContext>& context) const
{
  AWS_MAKE_ASYNC_OPERATION(CreateGateway, request, handler, context, m_executor.get());
}

DeleteGatewayOutcome BackupGatewayClient::DeleteGateway(const DeleteGatewayRequest& request) const
{
  AWS_OPERATION_CHECK_PTR(m_endpointProvider, DeleteGateway, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE);
  ResolveEndpointOutcome endpointResolutionOutcome = m_endpointProvider->ResolveEndpoint(request.GetEndpointContextParams());
  AWS_OPERATION_CHECK_SUCCESS(endpointResolutionOutcome, DeleteGateway, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE, endpointResolutionOutcome.GetError().GetMessage());
  return DeleteGatewayOutcome(MakeRequest(request, endpointResolutionOutcome.GetResult(), Aws::Http::HttpMethod::HTTP_POST, Aws::Auth::SIGV4_SIGNER));
}

DeleteGatewayOutcomeCallable BackupGatewayClient::DeleteGatewayCallable(const DeleteGatewayRequest& request) const
{
  AWS_MAKE_CALLABLE_OPERATION(DeleteGateway, request, m_executor.get());
}

void BackupGatewayClient::DeleteGatewayAsync(const DeleteGatewayRequest& request, const DeleteGatewayResponseReceivedHandler& handler, const std::shared_ptr<const Aws::Client::AsyncCallerContext>& context) const
{
  AWS_MAKE_ASYNC_OPERATION(DeleteGateway, request, handler, context, m_executor.get());
}

DeleteHypervisorOutcome BackupGatewayClient::DeleteHypervisor(const DeleteHypervisorRequest& request) const
{
  AWS_OPERATION_CHECK_PTR(m_endpointProvider, DeleteHypervisor, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE);
  ResolveEndpointOutcome endpointResolutionOutcome = m_endpointProvider->ResolveEndpoint(request.GetEndpointContextParams());
  AWS_OPERATION_CHECK_SUCCESS(endpointResolutionOutcome, DeleteHypervisor, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE, endpointResolutionOutcome.GetError().GetMessage());
  return DeleteHypervisorOutcome(MakeRequest(request, endpointResolutionOutcome.GetResult(), Aws::Http::HttpMethod::HTTP_POST, Aws::Auth::SIGV4_SIGNER));
}

DeleteHypervisorOutcomeCallable BackupGatewayClient::DeleteHypervisorCallable(const DeleteHypervisorRequest& request) const
{
  AWS_MAKE_CALLABLE_OPERATION(DeleteHypervisor, request, m_executor.get());
}

void BackupGatewayClient::DeleteHypervisorAsync(const DeleteHypervisorRequest& request, const DeleteHypervisorResponseReceivedHandler& handler, const std::shared_ptr<const Aws::Client::AsyncCallerContext>& context) const
{
  AWS_MAKE_ASYNC_OPERATION(DeleteHypervisor, request, handler, context, m_executor.get());
}

DisassociateGatewayFromServerOutcome BackupGatewayClient::DisassociateGatewayFromServer(const DisassociateGatewayFromServerRequest& request) const
{
  AWS_OPERATION_CHECK_PTR(m_endpointProvider, DisassociateGatewayFromServer, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE);
  ResolveEndpointOutcome endpointResolutionOutcome = m_endpointProvider->ResolveEndpoint(request.GetEndpointContextParams());
  AWS_OPERATION_CHECK_SUCCESS(endpointResolutionOutcome, DisassociateGatewayFromServer, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE, endpointResolutionOutcome.GetError().GetMessage());
  return DisassociateGatewayFromServerOutcome(MakeRequest(request, endpointResolutionOutcome.GetResult(), Aws::Http::HttpMethod::HTTP_POST, Aws::Auth::SIGV4_SIGNER));
}

DisassociateGatewayFromServerOutcomeCallable BackupGatewayClient::DisassociateGatewayFromServerCallable(const DisassociateGatewayFromServerRequest& request) const
{
  AWS_MAKE_CALLABLE_OPERATION(DisassociateGatewayFromServer, request, m_executor.get());
}

void BackupGatewayClient::DisassociateGatewayFromServerAsync(const DisassociateGatewayFromServerRequest& request, const DisassociateGatewayFromServerResponseReceivedHandler& handler, const std::shared_ptr<const Aws::Client::AsyncCallerContext>& context) const
{
  AWS_MAKE_ASYNC_OPERATION(DisassociateGatewayFromServer, request, handler, context, m_executor.get());
}

GetGatewayOutcome BackupGatewayClient::GetGateway(const GetGatewayRequest& request) const
{
  AWS_OPERATION_CHECK_PTR(m_endpointProvider, GetGateway, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE);
  ResolveEndpointOutcome endpointResolutionOutcome = m_endpointProvider->ResolveEndpoint(request.GetEndpointContextParams());
  AWS_OPERATION_CHECK_SUCCESS(endpointResolutionOutcome, GetGateway, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE, endpointResolutionOutcome.GetError().GetMessage());
  return GetGatewayOutcome(MakeRequest(request, endpointResolutionOutcome.GetResult(), Aws::Http::HttpMethod::HTTP_POST, Aws::Auth::SIGV4_SIGNER));
}

GetGatewayOutcomeCallable BackupGatewayClient::GetGatewayCallable(const GetGatewayRequest& request) const
{
  AWS_MAKE_CALLABLE_OPERATION(GetGateway, request, m_executor.get());
}

void BackupGatewayClient::GetGatewayAsync(const GetGatewayRequest& request, const GetGatewayResponseReceivedHandler& handler, const std::shared_ptr<const Aws::Client::AsyncCallerContext>& context) const
{
  AWS_MAKE_ASYNC_OPERATION(GetGateway, request, handler, context, m_executor.get());
}

GetVirtualMachineOutcome BackupGatewayClient::GetVirtualMachine(const GetVirtualMachineRequest& request) const
{
  AWS_OPERATION_CHECK_PTR(m_endpointProvider, GetVirtualMachine, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE);
  ResolveEndpointOutcome endpointResolutionOutcome = m_endpointProvider->ResolveEndpoint(request.GetEndpointContextParams());
  AWS_OPERATION_CHECK_SUCCESS(endpointResolutionOutcome, GetVirtualMachine, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE, endpointResolutionOutcome.GetError().GetMessage());
  return GetVirtualMachineOutcome(MakeRequest(request, endpointResolutionOutcome.GetResult(), Aws::Http::HttpMethod::HTTP_POST, Aws::Auth::SIGV4_SIGNER));
}

GetVirtualMachineOutcomeCallable BackupGatewayClient::GetVirtualMachineCallable(const GetVirtualMachineRequest& request) const
{
  AWS_MAKE_CALLABLE_OPERATION(GetVirtualMachine, request, m_executor.get());
}

void BackupGatewayClient::GetVirtualMachineAsync(const GetVirtualMachineRequest& request, const GetVirtualMachineResponseReceivedHandler& handler, const std::shared_ptr<const Aws::Client::AsyncCallerContext>& context) const
{
  AWS_MAKE_ASYNC_OPERATION(GetVirtualMachine, request, handler, context, m_executor.get());
}

ImportHypervisorConfigurationOutcome BackupGatewayClient::ImportHypervisorConfiguration(const ImportHypervisorConfigurationRequest& request) const
{
  AWS_OPERATION_CHECK_PTR(m_endpointProvider, ImportHypervisorConfiguration, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE);
  ResolveEndpointOutcome endpointResolutionOutcome = m_endpointProvider->ResolveEndpoint(request.GetEndpointContextParams());
  AWS_OPERATION_CHECK_SUCCESS(endpointResolutionOutcome, ImportHypervisorConfiguration, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE, endpointResolutionOutcome.GetError().GetMessage());
  return ImportHypervisorConfigurationOutcome(MakeRequest(request, endpointResolutionOutcome.GetResult(), Aws::Http::HttpMethod::HTTP_POST, Aws::Auth::SIGV4_SIGNER));
}

ImportHypervisorConfigurationOutcomeCallable BackupGatewayClient::ImportHypervisorConfigurationCallable(const ImportHypervisorConfigurationRequest& request) const
{
  AWS_MAKE_CALLABLE_OPERATION(ImportHypervisorConfiguration, request, m_executor.get());
}

void BackupGatewayClient::ImportHypervisorConfigurationAsync(const ImportHypervisorConfigurationRequest& request, const ImportHypervisorConfigurationResponseReceivedHandler& handler, const std::shared_ptr<const Aws::Client::AsyncCallerContext>& context) const
{
  AWS_MAKE_ASYNC_OPERATION(ImportHypervisorConfiguration, request, handler, context, m_executor.get());
}

ListGatewaysOutcome BackupGatewayClient::ListGateways(const ListGatewaysRequest& request) const
{
  AWS_OPERATION_CHECK_PTR(m_endpointProvider, ListGateways, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE);
  ResolveEndpointOutcome endpointResolutionOutcome = m_endpointProvider->ResolveEndpoint(request.GetEndpointContextParams());
  AWS_OPERATION_CHECK_SUCCESS(endpointResolutionOutcome, ListGateways, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE, endpointResolutionOutcome.GetError().GetMessage());
  return ListGatewaysOutcome(MakeRequest(request, endpointResolutionOutcome.GetResult(), Aws::Http::HttpMethod::HTTP_POST, Aws::Auth::SIGV4_SIGNER));
}

ListGatewaysOutcomeCallable BackupGatewayClient::ListGatewaysCallable(const ListGatewaysRequest& request) const
{
  AWS_MAKE_CALLABLE_OPERATION(ListGateways, request, m_executor.get());
}

void BackupGatewayClient::ListGatewaysAsync(const ListGatewaysRequest& request, const ListGatewaysResponseReceivedHandler& handler, const std::shared_ptr<const Aws::Client::AsyncCallerContext>& context) const
{
  AWS_MAKE_ASYNC_OPERATION(ListGateways, request, handler, context, m_executor.get());
}

ListHypervisorsOutcome BackupGatewayClient::ListHypervisors(const ListHypervisorsRequest& request) const
{
  AWS_OPERATION_CHECK_PTR(m_endpointProvider, ListHypervisors, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE);
  ResolveEndpointOutcome endpointResolutionOutcome = m_endpointProvider->ResolveEndpoint(request.GetEndpointContextParams());
  AWS_OPERATION_CHECK_SUCCESS(endpointResolutionOutcome, ListHypervisors, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE, endpointResolutionOutcome.GetError().GetMessage());
  return ListHypervisorsOutcome(MakeRequest(request, endpointResolutionOutcome.GetResult(), Aws::Http::HttpMethod::HTTP_POST, Aws::Auth::SIGV4_SIGNER));
}

ListHypervisorsOutcomeCallable BackupGatewayClient::ListHypervisorsCallable(const ListHypervisorsRequest& request) const
{
  AWS_MAKE_CALLABLE_OPERATION(ListHypervisors, request, m_executor.get());
}

void BackupGatewayClient::ListHypervisorsAsync(const ListHypervisorsRequest& request, const ListHypervisorsResponseReceivedHandler& handler, const std::shared_ptr<const Aws::Client::AsyncCallerContext>& context) const
{
  AWS_MAKE_ASYNC_OPERATION(ListHypervisors, request, handler, context, m_executor.get());
}

ListTagsForResourceOutcome BackupGatewayClient::ListTagsForResource(const ListTagsForResourceRequest& request) const
{
  AWS_OPERATION_CHECK_PTR(m_endpointProvider, ListTagsForResource, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE);
  ResolveEndpointOutcome endpointResolutionOutcome = m_endpointProvider->ResolveEndpoint(request.GetEndpointContextParams());
  AWS_OPERATION_CHECK_SUCCESS(endpointResolutionOutcome, ListTagsForResource, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE, endpointResolutionOutcome.GetError().GetMessage());
  return ListTagsForResourceOutcome(MakeRequest(request, endpointResolutionOutcome.GetResult(), Aws::Http::HttpMethod::HTTP_POST, Aws::Auth::SIGV4_SIGNER));
}

ListTagsForResourceOutcomeCallable BackupGatewayClient::ListTagsForResourceCallable(const ListTagsForResourceRequest& request) const
{
  AWS_MAKE_CALLABLE_OPERATION(ListTagsForResource, request, m_executor.get());
}

void BackupGatewayClient::ListTagsForResourceAsync(const ListTagsForResourceRequest& request, const ListTagsForResourceResponseReceivedHandler& handler, const std::shared_ptr<const Aws::Client::AsyncCallerContext>& context) const
{
  AWS_MAKE_ASYNC_OPERATION(ListTagsForResource, request, handler, context, m_executor.get());
}

ListVirtualMachinesOutcome BackupGatewayClient::ListVirtualMachines(const ListVirtualMachinesRequest& request) const
{
  AWS_OPERATION_CHECK_PTR(m_endpointProvider, ListVirtualMachines, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE);
  ResolveEndpointOutcome endpointResolutionOutcome = m_endpointProvider->ResolveEndpoint(request.GetEndpointContextParams());
  AWS_OPERATION_CHECK_SUCCESS(endpointResolutionOutcome, ListVirtualMachines, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE, endpointResolutionOutcome.GetError().GetMessage());
  return ListVirtualMachinesOutcome(MakeRequest(request, endpointResolutionOutcome.GetResult(), Aws::Http::HttpMethod::HTTP_POST, Aws::Auth::SIGV4_SIGNER));
}

ListVirtualMachinesOutcomeCallable BackupGatewayClient::ListVirtualMachinesCallable(const ListVirtualMachinesRequest& request) const
{
  AWS_MAKE_CALLABLE_OPERATION(ListVirtualMachines, request, m_executor.get());
}

void BackupGatewayClient::ListVirtualMachinesAsync(const ListVirtualMachinesRequest& request, const ListVirtualMachinesResponseReceivedHandler& handler, const std::shared_ptr<const Aws::Client::AsyncCallerContext>& context) const
{
  AWS_MAKE_ASYNC_OPERATION(ListVirtualMachines, request, handler, context, m_executor.get());
}

PutMaintenanceStartTimeOutcome BackupGatewayClient::PutMaintenanceStartTime(const PutMaintenanceStartTimeRequest& request) const
{
  AWS_OPERATION_CHECK_PTR(m_endpointProvider, PutMaintenanceStartTime, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE);
  ResolveEndpointOutcome endpointResolutionOutcome = m_endpointProvider->ResolveEndpoint(request.GetEndpointContextParams());
  AWS_OPERATION_CHECK_SUCCESS(endpointResolutionOutcome, PutMaintenanceStartTime, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE, endpointResolutionOutcome.GetError().GetMessage());
  return PutMaintenanceStartTimeOutcome(MakeRequest(request, endpointResolutionOutcome.GetResult(), Aws::Http::HttpMethod::HTTP_POST, Aws::Auth::SIGV4_SIGNER));
}

PutMaintenanceStartTimeOutcomeCallable BackupGatewayClient::PutMaintenanceStartTimeCallable(const PutMaintenanceStartTimeRequest& request) const
{
  AWS_MAKE_CALLABLE_OPERATION(PutMaintenanceStartTime, request, m_executor.get());
}

void BackupGatewayClient::PutMaintenanceStartTimeAsync(const PutMaintenanceStartTimeRequest& request, const PutMaintenanceStartTimeResponseReceivedHandler& handler, const std::shared_ptr<const Aws::Client::AsyncCallerContext>& context) const
{
  AWS_MAKE_ASYNC_OPERATION(PutMaintenanceStartTime, request, handler, context, m_executor.get());
}

TagResourceOutcome BackupGatewayClient::TagResource(const TagResourceRequest& request) const
{
  AWS_OPERATION_CHECK_PTR(m_endpointProvider, TagResource, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE);
  ResolveEndpointOutcome endpointResolutionOutcome = m_endpointProvider->ResolveEndpoint(request.GetEndpointContextParams());
  AWS_OPERATION_CHECK_SUCCESS(endpointResolutionOutcome, TagResource, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE, endpointResolutionOutcome.GetError().GetMessage());
  return TagResourceOutcome(MakeRequest(request, endpointResolutionOutcome.GetResult(), Aws::Http::HttpMethod::HTTP_POST, Aws::Auth::SIGV4_SIGNER));
}

TagResourceOutcomeCallable BackupGatewayClient::TagResourceCallable(const TagResourceRequest& request) const
{
  AWS_MAKE_CALLABLE_OPERATION(TagResource, request, m_executor.get());
}

void BackupGatewayClient::TagResourceAsync(const TagResourceRequest& request, const TagResourceResponseReceivedHandler& handler, const std::shared_ptr<const Aws::Client::AsyncCallerContext>& context) const
{
  AWS_MAKE_ASYNC_OPERATION(TagResource, request, handler, context, m_executor.get());
}

TestHypervisorConfigurationOutcome BackupGatewayClient::TestHypervisorConfiguration(const TestHypervisorConfigurationRequest& request) const
{
  AWS_OPERATION_CHECK_PTR(m_endpointProvider, TestHypervisorConfiguration, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE);
  ResolveEndpointOutcome endpointResolutionOutcome = m_endpointProvider->ResolveEndpoint(request.GetEndpointContextParams());
  AWS_OPERATION_CHECK_SUCCESS(endpointResolutionOutcome, TestHypervisorConfiguration, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE, endpointResolutionOutcome.GetError().GetMessage());
  return TestHypervisorConfigurationOutcome(MakeRequest(request, endpointResolutionOutcome.GetResult(), Aws::Http::HttpMethod::HTTP_POST, Aws::Auth::SIGV4_SIGNER));
}

TestHypervisorConfigurationOutcomeCallable BackupGatewayClient::TestHypervisorConfigurationCallable(const TestHypervisorConfigurationRequest& request) const
{
  AWS_MAKE_CALLABLE_OPERATION(TestHypervisorConfiguration, request, m_executor.get());
}

void BackupGatewayClient::TestHypervisorConfigurationAsync(const TestHypervisorConfigurationRequest& request, const TestHypervisorConfigurationResponseReceivedHandler& handler, const std::shared_ptr<const Aws::Client::AsyncCallerContext>& context) const
{
  AWS_MAKE_ASYNC_OPERATION(TestHypervisorConfiguration, request, handler, context, m_executor.get());
}

UntagResourceOutcome BackupGatewayClient::UntagResource(const UntagResourceRequest& request) const
{
  AWS_OPERATION_CHECK_PTR(m_endpointProvider, UntagResource, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE);
  ResolveEndpointOutcome endpointResolutionOutcome = m_endpointProvider->ResolveEndpoint(request.GetEndpointContextParams());
  AWS_OPERATION_CHECK_SUCCESS(endpointResolutionOutcome, UntagResource, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE, endpointResolutionOutcome.GetError().GetMessage());
  return UntagResourceOutcome(MakeRequest(request, endpointResolutionOutcome.GetResult(), Aws::Http::HttpMethod::HTTP_POST, Aws::Auth::SIGV4_SIGNER));
}

UntagResourceOutcomeCallable BackupGatewayClient::UntagResourceCallable(const UntagResourceRequest& request) const
{
  AWS_MAKE_CALLABLE_OPERATION(UntagResource, request, m_executor.get());
}

void BackupGatewayClient::UntagResourceAsync(const UntagResourceRequest& request, const UntagResourceResponseReceivedHandler& handler, const std::shared_ptr<const Aws::Client::AsyncCallerContext>& context) const
{
  AWS_MAKE_ASYNC_OPERATION(UntagResource, request, handler, context, m_executor.get());
}

UpdateGatewayInformationOutcome BackupGatewayClient::UpdateGatewayInformation(const UpdateGatewayInformationRequest& request) const
{
  AWS_OPERATION_CHECK_PTR(m_endpointProvider, UpdateGatewayInformation, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE);
  ResolveEndpointOutcome endpointResolutionOutcome = m_endpointProvider->ResolveEndpoint(request.GetEndpointContextParams());
  AWS_OPERATION_CHECK_SUCCESS(endpointResolutionOutcome, UpdateGatewayInformation, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE, endpointResolutionOutcome.GetError().GetMessage());
  return UpdateGatewayInformationOutcome(MakeRequest(request, endpointResolutionOutcome.GetResult(), Aws::Http::HttpMethod::HTTP_POST, Aws::Auth::SIGV4_SIGNER));
}

UpdateGatewayInformationOutcomeCallable BackupGatewayClient::UpdateGatewayInformationCallable(const UpdateGatewayInformationRequest& request) const
{
  AWS_MAKE_CALLABLE_OPERATION(UpdateGatewayInformation, request, m_executor.get());
}

void BackupGatewayClient::UpdateGatewayInformationAsync(const UpdateGatewayInformationRequest& request, const UpdateGatewayInformationResponseReceivedHandler& handler, const std::shared_ptr<const Aws::Client::AsyncCallerContext>& context) const
{
  AWS_MAKE_ASYNC_OPERATION(UpdateGatewayInformation, request, handler, context, m_executor.get());
}

UpdateGatewaySoftwareNowOutcome BackupGatewayClient::UpdateGatewaySoftwareNow(const UpdateGatewaySoftwareNowRequest& request) const
{
  AWS_OPERATION_CHECK_PTR(m_endpointProvider, UpdateGatewaySoftwareNow, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE);
  ResolveEndpointOutcome endpointResolutionOutcome = m_endpointProvider->ResolveEndpoint(request.GetEndpointContextParams());
  AWS_OPERATION_CHECK_SUCCESS(endpointResolutionOutcome, UpdateGatewaySoftwareNow, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE, endpointResolutionOutcome.GetError().GetMessage());
  return UpdateGatewaySoftwareNowOutcome(MakeRequest(request, endpointResolutionOutcome.GetResult(), Aws::Http::HttpMethod::HTTP_POST, Aws::Auth::SIGV4_SIGNER));
}

UpdateGatewaySoftwareNowOutcomeCallable BackupGatewayClient::UpdateGatewaySoftwareNowCallable(const UpdateGatewaySoftwareNowRequest& request) const
{
  AWS_MAKE_CALLABLE_OPERATION(UpdateGatewaySoftwareNow, request, m_executor.get());
}

void BackupGatewayClient::UpdateGatewaySoftwareNowAsync(const UpdateGatewaySoftwareNowRequest& request, const UpdateGatewaySoftwareNowResponseReceivedHandler& handler, const std::shared_ptr<const Aws::Client::AsyncCallerContext>& context) const
{
  AWS_MAKE_ASYNC_OPERATION(UpdateGatewaySoftwareNow, request, handler, context, m_executor.get());
}

UpdateHypervisorOutcome BackupGatewayClient::UpdateHypervisor(const UpdateHypervisorRequest& request) const
{
  AWS_OPERATION_CHECK_PTR(m_endpointProvider, UpdateHypervisor, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE);
  ResolveEndpointOutcome endpointResolutionOutcome = m_endpointProvider->ResolveEndpoint(request.GetEndpointContextParams());
  AWS_OPERATION_CHECK_SUCCESS(endpointResolutionOutcome, UpdateHypervisor, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE, endpointResolutionOutcome.GetError().GetMessage());
  return UpdateHypervisorOutcome(MakeRequest(request, endpointResolutionOutcome.GetResult(), Aws::Http::HttpMethod::HTTP_POST, Aws::Auth::SIGV4_SIGNER));
}

UpdateHypervisorOutcomeCallable BackupGatewayClient::UpdateHypervisorCallable(const UpdateHypervisorRequest& request) const
{
  AWS_MAKE_CALLABLE_OPERATION(UpdateHypervisor, request, m_executor.get());
}

void BackupGatewayClient::UpdateHypervisorAsync(const UpdateHypervisorRequest& request, const UpdateHypervisorResponseReceivedHandler& handler, const std::shared_ptr<const Aws::Client::AsyncCallerContext>& context) const
{
  AWS_MAKE_ASYNC_OPERATION(UpdateHypervisor, request, handler, context, m_executor.get());
}

