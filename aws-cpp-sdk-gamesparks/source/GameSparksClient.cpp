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

#include <aws/gamesparks/GameSparksClient.h>
#include <aws/gamesparks/GameSparksErrorMarshaller.h>
#include <aws/gamesparks/GameSparksEndpointProvider.h>
#include <aws/gamesparks/model/CreateGameRequest.h>
#include <aws/gamesparks/model/CreateSnapshotRequest.h>
#include <aws/gamesparks/model/CreateStageRequest.h>
#include <aws/gamesparks/model/DeleteGameRequest.h>
#include <aws/gamesparks/model/DeleteStageRequest.h>
#include <aws/gamesparks/model/DisconnectPlayerRequest.h>
#include <aws/gamesparks/model/ExportSnapshotRequest.h>
#include <aws/gamesparks/model/GetExtensionRequest.h>
#include <aws/gamesparks/model/GetExtensionVersionRequest.h>
#include <aws/gamesparks/model/GetGameRequest.h>
#include <aws/gamesparks/model/GetGameConfigurationRequest.h>
#include <aws/gamesparks/model/GetGeneratedCodeJobRequest.h>
#include <aws/gamesparks/model/GetPlayerConnectionStatusRequest.h>
#include <aws/gamesparks/model/GetSnapshotRequest.h>
#include <aws/gamesparks/model/GetStageRequest.h>
#include <aws/gamesparks/model/GetStageDeploymentRequest.h>
#include <aws/gamesparks/model/ImportGameConfigurationRequest.h>
#include <aws/gamesparks/model/ListExtensionVersionsRequest.h>
#include <aws/gamesparks/model/ListExtensionsRequest.h>
#include <aws/gamesparks/model/ListGamesRequest.h>
#include <aws/gamesparks/model/ListGeneratedCodeJobsRequest.h>
#include <aws/gamesparks/model/ListSnapshotsRequest.h>
#include <aws/gamesparks/model/ListStageDeploymentsRequest.h>
#include <aws/gamesparks/model/ListStagesRequest.h>
#include <aws/gamesparks/model/ListTagsForResourceRequest.h>
#include <aws/gamesparks/model/StartGeneratedCodeJobRequest.h>
#include <aws/gamesparks/model/StartStageDeploymentRequest.h>
#include <aws/gamesparks/model/TagResourceRequest.h>
#include <aws/gamesparks/model/UntagResourceRequest.h>
#include <aws/gamesparks/model/UpdateGameRequest.h>
#include <aws/gamesparks/model/UpdateGameConfigurationRequest.h>
#include <aws/gamesparks/model/UpdateSnapshotRequest.h>
#include <aws/gamesparks/model/UpdateStageRequest.h>

using namespace Aws;
using namespace Aws::Auth;
using namespace Aws::Client;
using namespace Aws::GameSparks;
using namespace Aws::GameSparks::Model;
using namespace Aws::Http;
using namespace Aws::Utils::Json;
using ResolveEndpointOutcome = Aws::Endpoint::ResolveEndpointOutcome;

const char* GameSparksClient::SERVICE_NAME = "gamesparks";
const char* GameSparksClient::ALLOCATION_TAG = "GameSparksClient";

GameSparksClient::GameSparksClient(const GameSparks::GameSparksClientConfiguration& clientConfiguration,
                                   std::shared_ptr<GameSparksEndpointProviderBase> endpointProvider) :
  BASECLASS(clientConfiguration,
            Aws::MakeShared<AWSAuthV4Signer>(ALLOCATION_TAG,
                                             Aws::MakeShared<DefaultAWSCredentialsProviderChain>(ALLOCATION_TAG),
                                             SERVICE_NAME,
                                             Aws::Region::ComputeSignerRegion(clientConfiguration.region)),
            Aws::MakeShared<GameSparksErrorMarshaller>(ALLOCATION_TAG)),
  m_clientConfiguration(clientConfiguration),
  m_executor(clientConfiguration.executor),
  m_endpointProvider(std::move(endpointProvider))
{
  init(m_clientConfiguration);
}

GameSparksClient::GameSparksClient(const AWSCredentials& credentials,
                                   std::shared_ptr<GameSparksEndpointProviderBase> endpointProvider,
                                   const GameSparks::GameSparksClientConfiguration& clientConfiguration) :
  BASECLASS(clientConfiguration,
            Aws::MakeShared<AWSAuthV4Signer>(ALLOCATION_TAG,
                                             Aws::MakeShared<SimpleAWSCredentialsProvider>(ALLOCATION_TAG, credentials),
                                             SERVICE_NAME,
                                             Aws::Region::ComputeSignerRegion(clientConfiguration.region)),
            Aws::MakeShared<GameSparksErrorMarshaller>(ALLOCATION_TAG)),
    m_clientConfiguration(clientConfiguration),
    m_executor(clientConfiguration.executor),
    m_endpointProvider(std::move(endpointProvider))
{
  init(m_clientConfiguration);
}

GameSparksClient::GameSparksClient(const std::shared_ptr<AWSCredentialsProvider>& credentialsProvider,
                                   std::shared_ptr<GameSparksEndpointProviderBase> endpointProvider,
                                   const GameSparks::GameSparksClientConfiguration& clientConfiguration) :
  BASECLASS(clientConfiguration,
            Aws::MakeShared<AWSAuthV4Signer>(ALLOCATION_TAG,
                                             credentialsProvider,
                                             SERVICE_NAME,
                                             Aws::Region::ComputeSignerRegion(clientConfiguration.region)),
            Aws::MakeShared<GameSparksErrorMarshaller>(ALLOCATION_TAG)),
    m_clientConfiguration(clientConfiguration),
    m_executor(clientConfiguration.executor),
    m_endpointProvider(std::move(endpointProvider))
{
  init(m_clientConfiguration);
}

    /* Legacy constructors due deprecation */
  GameSparksClient::GameSparksClient(const Client::ClientConfiguration& clientConfiguration) :
  BASECLASS(clientConfiguration,
            Aws::MakeShared<AWSAuthV4Signer>(ALLOCATION_TAG,
                                             Aws::MakeShared<DefaultAWSCredentialsProviderChain>(ALLOCATION_TAG),
                                             SERVICE_NAME,
                                             Aws::Region::ComputeSignerRegion(clientConfiguration.region)),
            Aws::MakeShared<GameSparksErrorMarshaller>(ALLOCATION_TAG)),
  m_clientConfiguration(clientConfiguration),
  m_executor(clientConfiguration.executor),
  m_endpointProvider(Aws::MakeShared<GameSparksEndpointProvider>(ALLOCATION_TAG))
{
  init(m_clientConfiguration);
}

GameSparksClient::GameSparksClient(const AWSCredentials& credentials,
                                   const Client::ClientConfiguration& clientConfiguration) :
  BASECLASS(clientConfiguration,
            Aws::MakeShared<AWSAuthV4Signer>(ALLOCATION_TAG,
                                             Aws::MakeShared<SimpleAWSCredentialsProvider>(ALLOCATION_TAG, credentials),
                                             SERVICE_NAME,
                                             Aws::Region::ComputeSignerRegion(clientConfiguration.region)),
            Aws::MakeShared<GameSparksErrorMarshaller>(ALLOCATION_TAG)),
    m_clientConfiguration(clientConfiguration),
    m_executor(clientConfiguration.executor),
    m_endpointProvider(Aws::MakeShared<GameSparksEndpointProvider>(ALLOCATION_TAG))
{
  init(m_clientConfiguration);
}

GameSparksClient::GameSparksClient(const std::shared_ptr<AWSCredentialsProvider>& credentialsProvider,
                                   const Client::ClientConfiguration& clientConfiguration) :
  BASECLASS(clientConfiguration,
            Aws::MakeShared<AWSAuthV4Signer>(ALLOCATION_TAG,
                                             credentialsProvider,
                                             SERVICE_NAME,
                                             Aws::Region::ComputeSignerRegion(clientConfiguration.region)),
            Aws::MakeShared<GameSparksErrorMarshaller>(ALLOCATION_TAG)),
    m_clientConfiguration(clientConfiguration),
    m_executor(clientConfiguration.executor),
    m_endpointProvider(Aws::MakeShared<GameSparksEndpointProvider>(ALLOCATION_TAG))
{
  init(m_clientConfiguration);
}

    /* End of legacy constructors due deprecation */
GameSparksClient::~GameSparksClient()
{
}

std::shared_ptr<GameSparksEndpointProviderBase>& GameSparksClient::accessEndpointProvider()
{
  return m_endpointProvider;
}

void GameSparksClient::init(const GameSparks::GameSparksClientConfiguration& config)
{
  AWSClient::SetServiceClientName("GameSparks");
  AWS_CHECK_PTR(SERVICE_NAME, m_endpointProvider);
  m_endpointProvider->InitBuiltInParameters(config);
}

void GameSparksClient::OverrideEndpoint(const Aws::String& endpoint)
{
  AWS_CHECK_PTR(SERVICE_NAME, m_endpointProvider);
  m_endpointProvider->OverrideEndpoint(endpoint);
}

CreateGameOutcome GameSparksClient::CreateGame(const CreateGameRequest& request) const
{
  AWS_OPERATION_CHECK_PTR(m_endpointProvider, CreateGame, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE);
  ResolveEndpointOutcome endpointResolutionOutcome = m_endpointProvider->ResolveEndpoint(request.GetEndpointContextParams());
  AWS_OPERATION_CHECK_SUCCESS(endpointResolutionOutcome, CreateGame, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE, endpointResolutionOutcome.GetError().GetMessage());
  endpointResolutionOutcome.GetResult().AddPathSegments("/game");
  return CreateGameOutcome(MakeRequest(request, endpointResolutionOutcome.GetResult(), Aws::Http::HttpMethod::HTTP_POST, Aws::Auth::SIGV4_SIGNER));
}

CreateGameOutcomeCallable GameSparksClient::CreateGameCallable(const CreateGameRequest& request) const
{
  return MakeCallableOperation(ALLOCATION_TAG, &GameSparksClient::CreateGame, this, request, m_executor.get());
}

void GameSparksClient::CreateGameAsync(const CreateGameRequest& request, const CreateGameResponseReceivedHandler& handler, const std::shared_ptr<const Aws::Client::AsyncCallerContext>& context) const
{
  MakeAsyncOperation(&GameSparksClient::CreateGame, this, request, handler, context, m_executor.get());
}

CreateSnapshotOutcome GameSparksClient::CreateSnapshot(const CreateSnapshotRequest& request) const
{
  AWS_OPERATION_CHECK_PTR(m_endpointProvider, CreateSnapshot, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE);
  if (!request.GameNameHasBeenSet())
  {
    AWS_LOGSTREAM_ERROR("CreateSnapshot", "Required field: GameName, is not set");
    return CreateSnapshotOutcome(Aws::Client::AWSError<GameSparksErrors>(GameSparksErrors::MISSING_PARAMETER, "MISSING_PARAMETER", "Missing required field [GameName]", false));
  }
  ResolveEndpointOutcome endpointResolutionOutcome = m_endpointProvider->ResolveEndpoint(request.GetEndpointContextParams());
  AWS_OPERATION_CHECK_SUCCESS(endpointResolutionOutcome, CreateSnapshot, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE, endpointResolutionOutcome.GetError().GetMessage());
  endpointResolutionOutcome.GetResult().AddPathSegments("/game/");
  endpointResolutionOutcome.GetResult().AddPathSegment(request.GetGameName());
  endpointResolutionOutcome.GetResult().AddPathSegments("/snapshot");
  return CreateSnapshotOutcome(MakeRequest(request, endpointResolutionOutcome.GetResult(), Aws::Http::HttpMethod::HTTP_POST, Aws::Auth::SIGV4_SIGNER));
}

CreateSnapshotOutcomeCallable GameSparksClient::CreateSnapshotCallable(const CreateSnapshotRequest& request) const
{
  return MakeCallableOperation(ALLOCATION_TAG, &GameSparksClient::CreateSnapshot, this, request, m_executor.get());
}

void GameSparksClient::CreateSnapshotAsync(const CreateSnapshotRequest& request, const CreateSnapshotResponseReceivedHandler& handler, const std::shared_ptr<const Aws::Client::AsyncCallerContext>& context) const
{
  MakeAsyncOperation(&GameSparksClient::CreateSnapshot, this, request, handler, context, m_executor.get());
}

CreateStageOutcome GameSparksClient::CreateStage(const CreateStageRequest& request) const
{
  AWS_OPERATION_CHECK_PTR(m_endpointProvider, CreateStage, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE);
  if (!request.GameNameHasBeenSet())
  {
    AWS_LOGSTREAM_ERROR("CreateStage", "Required field: GameName, is not set");
    return CreateStageOutcome(Aws::Client::AWSError<GameSparksErrors>(GameSparksErrors::MISSING_PARAMETER, "MISSING_PARAMETER", "Missing required field [GameName]", false));
  }
  ResolveEndpointOutcome endpointResolutionOutcome = m_endpointProvider->ResolveEndpoint(request.GetEndpointContextParams());
  AWS_OPERATION_CHECK_SUCCESS(endpointResolutionOutcome, CreateStage, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE, endpointResolutionOutcome.GetError().GetMessage());
  endpointResolutionOutcome.GetResult().AddPathSegments("/game/");
  endpointResolutionOutcome.GetResult().AddPathSegment(request.GetGameName());
  endpointResolutionOutcome.GetResult().AddPathSegments("/stage");
  return CreateStageOutcome(MakeRequest(request, endpointResolutionOutcome.GetResult(), Aws::Http::HttpMethod::HTTP_POST, Aws::Auth::SIGV4_SIGNER));
}

CreateStageOutcomeCallable GameSparksClient::CreateStageCallable(const CreateStageRequest& request) const
{
  return MakeCallableOperation(ALLOCATION_TAG, &GameSparksClient::CreateStage, this, request, m_executor.get());
}

void GameSparksClient::CreateStageAsync(const CreateStageRequest& request, const CreateStageResponseReceivedHandler& handler, const std::shared_ptr<const Aws::Client::AsyncCallerContext>& context) const
{
  MakeAsyncOperation(&GameSparksClient::CreateStage, this, request, handler, context, m_executor.get());
}

DeleteGameOutcome GameSparksClient::DeleteGame(const DeleteGameRequest& request) const
{
  AWS_OPERATION_CHECK_PTR(m_endpointProvider, DeleteGame, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE);
  if (!request.GameNameHasBeenSet())
  {
    AWS_LOGSTREAM_ERROR("DeleteGame", "Required field: GameName, is not set");
    return DeleteGameOutcome(Aws::Client::AWSError<GameSparksErrors>(GameSparksErrors::MISSING_PARAMETER, "MISSING_PARAMETER", "Missing required field [GameName]", false));
  }
  ResolveEndpointOutcome endpointResolutionOutcome = m_endpointProvider->ResolveEndpoint(request.GetEndpointContextParams());
  AWS_OPERATION_CHECK_SUCCESS(endpointResolutionOutcome, DeleteGame, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE, endpointResolutionOutcome.GetError().GetMessage());
  endpointResolutionOutcome.GetResult().AddPathSegments("/game/");
  endpointResolutionOutcome.GetResult().AddPathSegment(request.GetGameName());
  return DeleteGameOutcome(MakeRequest(request, endpointResolutionOutcome.GetResult(), Aws::Http::HttpMethod::HTTP_DELETE, Aws::Auth::SIGV4_SIGNER));
}

DeleteGameOutcomeCallable GameSparksClient::DeleteGameCallable(const DeleteGameRequest& request) const
{
  return MakeCallableOperation(ALLOCATION_TAG, &GameSparksClient::DeleteGame, this, request, m_executor.get());
}

void GameSparksClient::DeleteGameAsync(const DeleteGameRequest& request, const DeleteGameResponseReceivedHandler& handler, const std::shared_ptr<const Aws::Client::AsyncCallerContext>& context) const
{
  MakeAsyncOperation(&GameSparksClient::DeleteGame, this, request, handler, context, m_executor.get());
}

DeleteStageOutcome GameSparksClient::DeleteStage(const DeleteStageRequest& request) const
{
  AWS_OPERATION_CHECK_PTR(m_endpointProvider, DeleteStage, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE);
  if (!request.GameNameHasBeenSet())
  {
    AWS_LOGSTREAM_ERROR("DeleteStage", "Required field: GameName, is not set");
    return DeleteStageOutcome(Aws::Client::AWSError<GameSparksErrors>(GameSparksErrors::MISSING_PARAMETER, "MISSING_PARAMETER", "Missing required field [GameName]", false));
  }
  if (!request.StageNameHasBeenSet())
  {
    AWS_LOGSTREAM_ERROR("DeleteStage", "Required field: StageName, is not set");
    return DeleteStageOutcome(Aws::Client::AWSError<GameSparksErrors>(GameSparksErrors::MISSING_PARAMETER, "MISSING_PARAMETER", "Missing required field [StageName]", false));
  }
  ResolveEndpointOutcome endpointResolutionOutcome = m_endpointProvider->ResolveEndpoint(request.GetEndpointContextParams());
  AWS_OPERATION_CHECK_SUCCESS(endpointResolutionOutcome, DeleteStage, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE, endpointResolutionOutcome.GetError().GetMessage());
  endpointResolutionOutcome.GetResult().AddPathSegments("/game/");
  endpointResolutionOutcome.GetResult().AddPathSegment(request.GetGameName());
  endpointResolutionOutcome.GetResult().AddPathSegments("/stage/");
  endpointResolutionOutcome.GetResult().AddPathSegment(request.GetStageName());
  return DeleteStageOutcome(MakeRequest(request, endpointResolutionOutcome.GetResult(), Aws::Http::HttpMethod::HTTP_DELETE, Aws::Auth::SIGV4_SIGNER));
}

DeleteStageOutcomeCallable GameSparksClient::DeleteStageCallable(const DeleteStageRequest& request) const
{
  return MakeCallableOperation(ALLOCATION_TAG, &GameSparksClient::DeleteStage, this, request, m_executor.get());
}

void GameSparksClient::DeleteStageAsync(const DeleteStageRequest& request, const DeleteStageResponseReceivedHandler& handler, const std::shared_ptr<const Aws::Client::AsyncCallerContext>& context) const
{
  MakeAsyncOperation(&GameSparksClient::DeleteStage, this, request, handler, context, m_executor.get());
}

DisconnectPlayerOutcome GameSparksClient::DisconnectPlayer(const DisconnectPlayerRequest& request) const
{
  AWS_OPERATION_CHECK_PTR(m_endpointProvider, DisconnectPlayer, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE);
  if (!request.GameNameHasBeenSet())
  {
    AWS_LOGSTREAM_ERROR("DisconnectPlayer", "Required field: GameName, is not set");
    return DisconnectPlayerOutcome(Aws::Client::AWSError<GameSparksErrors>(GameSparksErrors::MISSING_PARAMETER, "MISSING_PARAMETER", "Missing required field [GameName]", false));
  }
  if (!request.PlayerIdHasBeenSet())
  {
    AWS_LOGSTREAM_ERROR("DisconnectPlayer", "Required field: PlayerId, is not set");
    return DisconnectPlayerOutcome(Aws::Client::AWSError<GameSparksErrors>(GameSparksErrors::MISSING_PARAMETER, "MISSING_PARAMETER", "Missing required field [PlayerId]", false));
  }
  if (!request.StageNameHasBeenSet())
  {
    AWS_LOGSTREAM_ERROR("DisconnectPlayer", "Required field: StageName, is not set");
    return DisconnectPlayerOutcome(Aws::Client::AWSError<GameSparksErrors>(GameSparksErrors::MISSING_PARAMETER, "MISSING_PARAMETER", "Missing required field [StageName]", false));
  }
  ResolveEndpointOutcome endpointResolutionOutcome = m_endpointProvider->ResolveEndpoint(request.GetEndpointContextParams());
  AWS_OPERATION_CHECK_SUCCESS(endpointResolutionOutcome, DisconnectPlayer, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE, endpointResolutionOutcome.GetError().GetMessage());
  endpointResolutionOutcome.GetResult().AddPathSegments("/runtime/game/");
  endpointResolutionOutcome.GetResult().AddPathSegment(request.GetGameName());
  endpointResolutionOutcome.GetResult().AddPathSegments("/stage/");
  endpointResolutionOutcome.GetResult().AddPathSegment(request.GetStageName());
  endpointResolutionOutcome.GetResult().AddPathSegments("/player/");
  endpointResolutionOutcome.GetResult().AddPathSegment(request.GetPlayerId());
  endpointResolutionOutcome.GetResult().AddPathSegments("/disconnect");
  return DisconnectPlayerOutcome(MakeRequest(request, endpointResolutionOutcome.GetResult(), Aws::Http::HttpMethod::HTTP_POST, Aws::Auth::SIGV4_SIGNER));
}

DisconnectPlayerOutcomeCallable GameSparksClient::DisconnectPlayerCallable(const DisconnectPlayerRequest& request) const
{
  return MakeCallableOperation(ALLOCATION_TAG, &GameSparksClient::DisconnectPlayer, this, request, m_executor.get());
}

void GameSparksClient::DisconnectPlayerAsync(const DisconnectPlayerRequest& request, const DisconnectPlayerResponseReceivedHandler& handler, const std::shared_ptr<const Aws::Client::AsyncCallerContext>& context) const
{
  MakeAsyncOperation(&GameSparksClient::DisconnectPlayer, this, request, handler, context, m_executor.get());
}

ExportSnapshotOutcome GameSparksClient::ExportSnapshot(const ExportSnapshotRequest& request) const
{
  AWS_OPERATION_CHECK_PTR(m_endpointProvider, ExportSnapshot, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE);
  if (!request.GameNameHasBeenSet())
  {
    AWS_LOGSTREAM_ERROR("ExportSnapshot", "Required field: GameName, is not set");
    return ExportSnapshotOutcome(Aws::Client::AWSError<GameSparksErrors>(GameSparksErrors::MISSING_PARAMETER, "MISSING_PARAMETER", "Missing required field [GameName]", false));
  }
  if (!request.SnapshotIdHasBeenSet())
  {
    AWS_LOGSTREAM_ERROR("ExportSnapshot", "Required field: SnapshotId, is not set");
    return ExportSnapshotOutcome(Aws::Client::AWSError<GameSparksErrors>(GameSparksErrors::MISSING_PARAMETER, "MISSING_PARAMETER", "Missing required field [SnapshotId]", false));
  }
  ResolveEndpointOutcome endpointResolutionOutcome = m_endpointProvider->ResolveEndpoint(request.GetEndpointContextParams());
  AWS_OPERATION_CHECK_SUCCESS(endpointResolutionOutcome, ExportSnapshot, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE, endpointResolutionOutcome.GetError().GetMessage());
  endpointResolutionOutcome.GetResult().AddPathSegments("/game/");
  endpointResolutionOutcome.GetResult().AddPathSegment(request.GetGameName());
  endpointResolutionOutcome.GetResult().AddPathSegments("/snapshot/");
  endpointResolutionOutcome.GetResult().AddPathSegment(request.GetSnapshotId());
  endpointResolutionOutcome.GetResult().AddPathSegments("/export");
  return ExportSnapshotOutcome(MakeRequest(request, endpointResolutionOutcome.GetResult(), Aws::Http::HttpMethod::HTTP_GET, Aws::Auth::SIGV4_SIGNER));
}

ExportSnapshotOutcomeCallable GameSparksClient::ExportSnapshotCallable(const ExportSnapshotRequest& request) const
{
  return MakeCallableOperation(ALLOCATION_TAG, &GameSparksClient::ExportSnapshot, this, request, m_executor.get());
}

void GameSparksClient::ExportSnapshotAsync(const ExportSnapshotRequest& request, const ExportSnapshotResponseReceivedHandler& handler, const std::shared_ptr<const Aws::Client::AsyncCallerContext>& context) const
{
  MakeAsyncOperation(&GameSparksClient::ExportSnapshot, this, request, handler, context, m_executor.get());
}

GetExtensionOutcome GameSparksClient::GetExtension(const GetExtensionRequest& request) const
{
  AWS_OPERATION_CHECK_PTR(m_endpointProvider, GetExtension, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE);
  if (!request.NameHasBeenSet())
  {
    AWS_LOGSTREAM_ERROR("GetExtension", "Required field: Name, is not set");
    return GetExtensionOutcome(Aws::Client::AWSError<GameSparksErrors>(GameSparksErrors::MISSING_PARAMETER, "MISSING_PARAMETER", "Missing required field [Name]", false));
  }
  if (!request.NamespaceHasBeenSet())
  {
    AWS_LOGSTREAM_ERROR("GetExtension", "Required field: Namespace, is not set");
    return GetExtensionOutcome(Aws::Client::AWSError<GameSparksErrors>(GameSparksErrors::MISSING_PARAMETER, "MISSING_PARAMETER", "Missing required field [Namespace]", false));
  }
  ResolveEndpointOutcome endpointResolutionOutcome = m_endpointProvider->ResolveEndpoint(request.GetEndpointContextParams());
  AWS_OPERATION_CHECK_SUCCESS(endpointResolutionOutcome, GetExtension, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE, endpointResolutionOutcome.GetError().GetMessage());
  endpointResolutionOutcome.GetResult().AddPathSegments("/extension/");
  endpointResolutionOutcome.GetResult().AddPathSegment(request.GetNamespace());
  endpointResolutionOutcome.GetResult().AddPathSegment(request.GetName());
  return GetExtensionOutcome(MakeRequest(request, endpointResolutionOutcome.GetResult(), Aws::Http::HttpMethod::HTTP_GET, Aws::Auth::SIGV4_SIGNER));
}

GetExtensionOutcomeCallable GameSparksClient::GetExtensionCallable(const GetExtensionRequest& request) const
{
  return MakeCallableOperation(ALLOCATION_TAG, &GameSparksClient::GetExtension, this, request, m_executor.get());
}

void GameSparksClient::GetExtensionAsync(const GetExtensionRequest& request, const GetExtensionResponseReceivedHandler& handler, const std::shared_ptr<const Aws::Client::AsyncCallerContext>& context) const
{
  MakeAsyncOperation(&GameSparksClient::GetExtension, this, request, handler, context, m_executor.get());
}

GetExtensionVersionOutcome GameSparksClient::GetExtensionVersion(const GetExtensionVersionRequest& request) const
{
  AWS_OPERATION_CHECK_PTR(m_endpointProvider, GetExtensionVersion, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE);
  if (!request.ExtensionVersionHasBeenSet())
  {
    AWS_LOGSTREAM_ERROR("GetExtensionVersion", "Required field: ExtensionVersion, is not set");
    return GetExtensionVersionOutcome(Aws::Client::AWSError<GameSparksErrors>(GameSparksErrors::MISSING_PARAMETER, "MISSING_PARAMETER", "Missing required field [ExtensionVersion]", false));
  }
  if (!request.NameHasBeenSet())
  {
    AWS_LOGSTREAM_ERROR("GetExtensionVersion", "Required field: Name, is not set");
    return GetExtensionVersionOutcome(Aws::Client::AWSError<GameSparksErrors>(GameSparksErrors::MISSING_PARAMETER, "MISSING_PARAMETER", "Missing required field [Name]", false));
  }
  if (!request.NamespaceHasBeenSet())
  {
    AWS_LOGSTREAM_ERROR("GetExtensionVersion", "Required field: Namespace, is not set");
    return GetExtensionVersionOutcome(Aws::Client::AWSError<GameSparksErrors>(GameSparksErrors::MISSING_PARAMETER, "MISSING_PARAMETER", "Missing required field [Namespace]", false));
  }
  ResolveEndpointOutcome endpointResolutionOutcome = m_endpointProvider->ResolveEndpoint(request.GetEndpointContextParams());
  AWS_OPERATION_CHECK_SUCCESS(endpointResolutionOutcome, GetExtensionVersion, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE, endpointResolutionOutcome.GetError().GetMessage());
  endpointResolutionOutcome.GetResult().AddPathSegments("/extension/");
  endpointResolutionOutcome.GetResult().AddPathSegment(request.GetNamespace());
  endpointResolutionOutcome.GetResult().AddPathSegment(request.GetName());
  endpointResolutionOutcome.GetResult().AddPathSegments("/version/");
  endpointResolutionOutcome.GetResult().AddPathSegment(request.GetExtensionVersion());
  return GetExtensionVersionOutcome(MakeRequest(request, endpointResolutionOutcome.GetResult(), Aws::Http::HttpMethod::HTTP_GET, Aws::Auth::SIGV4_SIGNER));
}

GetExtensionVersionOutcomeCallable GameSparksClient::GetExtensionVersionCallable(const GetExtensionVersionRequest& request) const
{
  return MakeCallableOperation(ALLOCATION_TAG, &GameSparksClient::GetExtensionVersion, this, request, m_executor.get());
}

void GameSparksClient::GetExtensionVersionAsync(const GetExtensionVersionRequest& request, const GetExtensionVersionResponseReceivedHandler& handler, const std::shared_ptr<const Aws::Client::AsyncCallerContext>& context) const
{
  MakeAsyncOperation(&GameSparksClient::GetExtensionVersion, this, request, handler, context, m_executor.get());
}

GetGameOutcome GameSparksClient::GetGame(const GetGameRequest& request) const
{
  AWS_OPERATION_CHECK_PTR(m_endpointProvider, GetGame, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE);
  if (!request.GameNameHasBeenSet())
  {
    AWS_LOGSTREAM_ERROR("GetGame", "Required field: GameName, is not set");
    return GetGameOutcome(Aws::Client::AWSError<GameSparksErrors>(GameSparksErrors::MISSING_PARAMETER, "MISSING_PARAMETER", "Missing required field [GameName]", false));
  }
  ResolveEndpointOutcome endpointResolutionOutcome = m_endpointProvider->ResolveEndpoint(request.GetEndpointContextParams());
  AWS_OPERATION_CHECK_SUCCESS(endpointResolutionOutcome, GetGame, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE, endpointResolutionOutcome.GetError().GetMessage());
  endpointResolutionOutcome.GetResult().AddPathSegments("/game/");
  endpointResolutionOutcome.GetResult().AddPathSegment(request.GetGameName());
  return GetGameOutcome(MakeRequest(request, endpointResolutionOutcome.GetResult(), Aws::Http::HttpMethod::HTTP_GET, Aws::Auth::SIGV4_SIGNER));
}

GetGameOutcomeCallable GameSparksClient::GetGameCallable(const GetGameRequest& request) const
{
  return MakeCallableOperation(ALLOCATION_TAG, &GameSparksClient::GetGame, this, request, m_executor.get());
}

void GameSparksClient::GetGameAsync(const GetGameRequest& request, const GetGameResponseReceivedHandler& handler, const std::shared_ptr<const Aws::Client::AsyncCallerContext>& context) const
{
  MakeAsyncOperation(&GameSparksClient::GetGame, this, request, handler, context, m_executor.get());
}

GetGameConfigurationOutcome GameSparksClient::GetGameConfiguration(const GetGameConfigurationRequest& request) const
{
  AWS_OPERATION_CHECK_PTR(m_endpointProvider, GetGameConfiguration, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE);
  if (!request.GameNameHasBeenSet())
  {
    AWS_LOGSTREAM_ERROR("GetGameConfiguration", "Required field: GameName, is not set");
    return GetGameConfigurationOutcome(Aws::Client::AWSError<GameSparksErrors>(GameSparksErrors::MISSING_PARAMETER, "MISSING_PARAMETER", "Missing required field [GameName]", false));
  }
  ResolveEndpointOutcome endpointResolutionOutcome = m_endpointProvider->ResolveEndpoint(request.GetEndpointContextParams());
  AWS_OPERATION_CHECK_SUCCESS(endpointResolutionOutcome, GetGameConfiguration, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE, endpointResolutionOutcome.GetError().GetMessage());
  endpointResolutionOutcome.GetResult().AddPathSegments("/game/");
  endpointResolutionOutcome.GetResult().AddPathSegment(request.GetGameName());
  endpointResolutionOutcome.GetResult().AddPathSegments("/configuration");
  return GetGameConfigurationOutcome(MakeRequest(request, endpointResolutionOutcome.GetResult(), Aws::Http::HttpMethod::HTTP_GET, Aws::Auth::SIGV4_SIGNER));
}

GetGameConfigurationOutcomeCallable GameSparksClient::GetGameConfigurationCallable(const GetGameConfigurationRequest& request) const
{
  return MakeCallableOperation(ALLOCATION_TAG, &GameSparksClient::GetGameConfiguration, this, request, m_executor.get());
}

void GameSparksClient::GetGameConfigurationAsync(const GetGameConfigurationRequest& request, const GetGameConfigurationResponseReceivedHandler& handler, const std::shared_ptr<const Aws::Client::AsyncCallerContext>& context) const
{
  MakeAsyncOperation(&GameSparksClient::GetGameConfiguration, this, request, handler, context, m_executor.get());
}

GetGeneratedCodeJobOutcome GameSparksClient::GetGeneratedCodeJob(const GetGeneratedCodeJobRequest& request) const
{
  AWS_OPERATION_CHECK_PTR(m_endpointProvider, GetGeneratedCodeJob, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE);
  if (!request.GameNameHasBeenSet())
  {
    AWS_LOGSTREAM_ERROR("GetGeneratedCodeJob", "Required field: GameName, is not set");
    return GetGeneratedCodeJobOutcome(Aws::Client::AWSError<GameSparksErrors>(GameSparksErrors::MISSING_PARAMETER, "MISSING_PARAMETER", "Missing required field [GameName]", false));
  }
  if (!request.JobIdHasBeenSet())
  {
    AWS_LOGSTREAM_ERROR("GetGeneratedCodeJob", "Required field: JobId, is not set");
    return GetGeneratedCodeJobOutcome(Aws::Client::AWSError<GameSparksErrors>(GameSparksErrors::MISSING_PARAMETER, "MISSING_PARAMETER", "Missing required field [JobId]", false));
  }
  if (!request.SnapshotIdHasBeenSet())
  {
    AWS_LOGSTREAM_ERROR("GetGeneratedCodeJob", "Required field: SnapshotId, is not set");
    return GetGeneratedCodeJobOutcome(Aws::Client::AWSError<GameSparksErrors>(GameSparksErrors::MISSING_PARAMETER, "MISSING_PARAMETER", "Missing required field [SnapshotId]", false));
  }
  ResolveEndpointOutcome endpointResolutionOutcome = m_endpointProvider->ResolveEndpoint(request.GetEndpointContextParams());
  AWS_OPERATION_CHECK_SUCCESS(endpointResolutionOutcome, GetGeneratedCodeJob, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE, endpointResolutionOutcome.GetError().GetMessage());
  endpointResolutionOutcome.GetResult().AddPathSegments("/game/");
  endpointResolutionOutcome.GetResult().AddPathSegment(request.GetGameName());
  endpointResolutionOutcome.GetResult().AddPathSegments("/snapshot/");
  endpointResolutionOutcome.GetResult().AddPathSegment(request.GetSnapshotId());
  endpointResolutionOutcome.GetResult().AddPathSegments("/generated-sdk-code-job/");
  endpointResolutionOutcome.GetResult().AddPathSegment(request.GetJobId());
  return GetGeneratedCodeJobOutcome(MakeRequest(request, endpointResolutionOutcome.GetResult(), Aws::Http::HttpMethod::HTTP_GET, Aws::Auth::SIGV4_SIGNER));
}

GetGeneratedCodeJobOutcomeCallable GameSparksClient::GetGeneratedCodeJobCallable(const GetGeneratedCodeJobRequest& request) const
{
  return MakeCallableOperation(ALLOCATION_TAG, &GameSparksClient::GetGeneratedCodeJob, this, request, m_executor.get());
}

void GameSparksClient::GetGeneratedCodeJobAsync(const GetGeneratedCodeJobRequest& request, const GetGeneratedCodeJobResponseReceivedHandler& handler, const std::shared_ptr<const Aws::Client::AsyncCallerContext>& context) const
{
  MakeAsyncOperation(&GameSparksClient::GetGeneratedCodeJob, this, request, handler, context, m_executor.get());
}

GetPlayerConnectionStatusOutcome GameSparksClient::GetPlayerConnectionStatus(const GetPlayerConnectionStatusRequest& request) const
{
  AWS_OPERATION_CHECK_PTR(m_endpointProvider, GetPlayerConnectionStatus, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE);
  if (!request.GameNameHasBeenSet())
  {
    AWS_LOGSTREAM_ERROR("GetPlayerConnectionStatus", "Required field: GameName, is not set");
    return GetPlayerConnectionStatusOutcome(Aws::Client::AWSError<GameSparksErrors>(GameSparksErrors::MISSING_PARAMETER, "MISSING_PARAMETER", "Missing required field [GameName]", false));
  }
  if (!request.PlayerIdHasBeenSet())
  {
    AWS_LOGSTREAM_ERROR("GetPlayerConnectionStatus", "Required field: PlayerId, is not set");
    return GetPlayerConnectionStatusOutcome(Aws::Client::AWSError<GameSparksErrors>(GameSparksErrors::MISSING_PARAMETER, "MISSING_PARAMETER", "Missing required field [PlayerId]", false));
  }
  if (!request.StageNameHasBeenSet())
  {
    AWS_LOGSTREAM_ERROR("GetPlayerConnectionStatus", "Required field: StageName, is not set");
    return GetPlayerConnectionStatusOutcome(Aws::Client::AWSError<GameSparksErrors>(GameSparksErrors::MISSING_PARAMETER, "MISSING_PARAMETER", "Missing required field [StageName]", false));
  }
  ResolveEndpointOutcome endpointResolutionOutcome = m_endpointProvider->ResolveEndpoint(request.GetEndpointContextParams());
  AWS_OPERATION_CHECK_SUCCESS(endpointResolutionOutcome, GetPlayerConnectionStatus, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE, endpointResolutionOutcome.GetError().GetMessage());
  endpointResolutionOutcome.GetResult().AddPathSegments("/runtime/game/");
  endpointResolutionOutcome.GetResult().AddPathSegment(request.GetGameName());
  endpointResolutionOutcome.GetResult().AddPathSegments("/stage/");
  endpointResolutionOutcome.GetResult().AddPathSegment(request.GetStageName());
  endpointResolutionOutcome.GetResult().AddPathSegments("/player/");
  endpointResolutionOutcome.GetResult().AddPathSegment(request.GetPlayerId());
  endpointResolutionOutcome.GetResult().AddPathSegments("/connection");
  return GetPlayerConnectionStatusOutcome(MakeRequest(request, endpointResolutionOutcome.GetResult(), Aws::Http::HttpMethod::HTTP_GET, Aws::Auth::SIGV4_SIGNER));
}

GetPlayerConnectionStatusOutcomeCallable GameSparksClient::GetPlayerConnectionStatusCallable(const GetPlayerConnectionStatusRequest& request) const
{
  return MakeCallableOperation(ALLOCATION_TAG, &GameSparksClient::GetPlayerConnectionStatus, this, request, m_executor.get());
}

void GameSparksClient::GetPlayerConnectionStatusAsync(const GetPlayerConnectionStatusRequest& request, const GetPlayerConnectionStatusResponseReceivedHandler& handler, const std::shared_ptr<const Aws::Client::AsyncCallerContext>& context) const
{
  MakeAsyncOperation(&GameSparksClient::GetPlayerConnectionStatus, this, request, handler, context, m_executor.get());
}

GetSnapshotOutcome GameSparksClient::GetSnapshot(const GetSnapshotRequest& request) const
{
  AWS_OPERATION_CHECK_PTR(m_endpointProvider, GetSnapshot, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE);
  if (!request.GameNameHasBeenSet())
  {
    AWS_LOGSTREAM_ERROR("GetSnapshot", "Required field: GameName, is not set");
    return GetSnapshotOutcome(Aws::Client::AWSError<GameSparksErrors>(GameSparksErrors::MISSING_PARAMETER, "MISSING_PARAMETER", "Missing required field [GameName]", false));
  }
  if (!request.SnapshotIdHasBeenSet())
  {
    AWS_LOGSTREAM_ERROR("GetSnapshot", "Required field: SnapshotId, is not set");
    return GetSnapshotOutcome(Aws::Client::AWSError<GameSparksErrors>(GameSparksErrors::MISSING_PARAMETER, "MISSING_PARAMETER", "Missing required field [SnapshotId]", false));
  }
  ResolveEndpointOutcome endpointResolutionOutcome = m_endpointProvider->ResolveEndpoint(request.GetEndpointContextParams());
  AWS_OPERATION_CHECK_SUCCESS(endpointResolutionOutcome, GetSnapshot, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE, endpointResolutionOutcome.GetError().GetMessage());
  endpointResolutionOutcome.GetResult().AddPathSegments("/game/");
  endpointResolutionOutcome.GetResult().AddPathSegment(request.GetGameName());
  endpointResolutionOutcome.GetResult().AddPathSegments("/snapshot/");
  endpointResolutionOutcome.GetResult().AddPathSegment(request.GetSnapshotId());
  return GetSnapshotOutcome(MakeRequest(request, endpointResolutionOutcome.GetResult(), Aws::Http::HttpMethod::HTTP_GET, Aws::Auth::SIGV4_SIGNER));
}

GetSnapshotOutcomeCallable GameSparksClient::GetSnapshotCallable(const GetSnapshotRequest& request) const
{
  return MakeCallableOperation(ALLOCATION_TAG, &GameSparksClient::GetSnapshot, this, request, m_executor.get());
}

void GameSparksClient::GetSnapshotAsync(const GetSnapshotRequest& request, const GetSnapshotResponseReceivedHandler& handler, const std::shared_ptr<const Aws::Client::AsyncCallerContext>& context) const
{
  MakeAsyncOperation(&GameSparksClient::GetSnapshot, this, request, handler, context, m_executor.get());
}

GetStageOutcome GameSparksClient::GetStage(const GetStageRequest& request) const
{
  AWS_OPERATION_CHECK_PTR(m_endpointProvider, GetStage, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE);
  if (!request.GameNameHasBeenSet())
  {
    AWS_LOGSTREAM_ERROR("GetStage", "Required field: GameName, is not set");
    return GetStageOutcome(Aws::Client::AWSError<GameSparksErrors>(GameSparksErrors::MISSING_PARAMETER, "MISSING_PARAMETER", "Missing required field [GameName]", false));
  }
  if (!request.StageNameHasBeenSet())
  {
    AWS_LOGSTREAM_ERROR("GetStage", "Required field: StageName, is not set");
    return GetStageOutcome(Aws::Client::AWSError<GameSparksErrors>(GameSparksErrors::MISSING_PARAMETER, "MISSING_PARAMETER", "Missing required field [StageName]", false));
  }
  ResolveEndpointOutcome endpointResolutionOutcome = m_endpointProvider->ResolveEndpoint(request.GetEndpointContextParams());
  AWS_OPERATION_CHECK_SUCCESS(endpointResolutionOutcome, GetStage, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE, endpointResolutionOutcome.GetError().GetMessage());
  endpointResolutionOutcome.GetResult().AddPathSegments("/game/");
  endpointResolutionOutcome.GetResult().AddPathSegment(request.GetGameName());
  endpointResolutionOutcome.GetResult().AddPathSegments("/stage/");
  endpointResolutionOutcome.GetResult().AddPathSegment(request.GetStageName());
  return GetStageOutcome(MakeRequest(request, endpointResolutionOutcome.GetResult(), Aws::Http::HttpMethod::HTTP_GET, Aws::Auth::SIGV4_SIGNER));
}

GetStageOutcomeCallable GameSparksClient::GetStageCallable(const GetStageRequest& request) const
{
  return MakeCallableOperation(ALLOCATION_TAG, &GameSparksClient::GetStage, this, request, m_executor.get());
}

void GameSparksClient::GetStageAsync(const GetStageRequest& request, const GetStageResponseReceivedHandler& handler, const std::shared_ptr<const Aws::Client::AsyncCallerContext>& context) const
{
  MakeAsyncOperation(&GameSparksClient::GetStage, this, request, handler, context, m_executor.get());
}

GetStageDeploymentOutcome GameSparksClient::GetStageDeployment(const GetStageDeploymentRequest& request) const
{
  AWS_OPERATION_CHECK_PTR(m_endpointProvider, GetStageDeployment, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE);
  if (!request.GameNameHasBeenSet())
  {
    AWS_LOGSTREAM_ERROR("GetStageDeployment", "Required field: GameName, is not set");
    return GetStageDeploymentOutcome(Aws::Client::AWSError<GameSparksErrors>(GameSparksErrors::MISSING_PARAMETER, "MISSING_PARAMETER", "Missing required field [GameName]", false));
  }
  if (!request.StageNameHasBeenSet())
  {
    AWS_LOGSTREAM_ERROR("GetStageDeployment", "Required field: StageName, is not set");
    return GetStageDeploymentOutcome(Aws::Client::AWSError<GameSparksErrors>(GameSparksErrors::MISSING_PARAMETER, "MISSING_PARAMETER", "Missing required field [StageName]", false));
  }
  ResolveEndpointOutcome endpointResolutionOutcome = m_endpointProvider->ResolveEndpoint(request.GetEndpointContextParams());
  AWS_OPERATION_CHECK_SUCCESS(endpointResolutionOutcome, GetStageDeployment, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE, endpointResolutionOutcome.GetError().GetMessage());
  endpointResolutionOutcome.GetResult().AddPathSegments("/game/");
  endpointResolutionOutcome.GetResult().AddPathSegment(request.GetGameName());
  endpointResolutionOutcome.GetResult().AddPathSegments("/stage/");
  endpointResolutionOutcome.GetResult().AddPathSegment(request.GetStageName());
  endpointResolutionOutcome.GetResult().AddPathSegments("/deployment");
  return GetStageDeploymentOutcome(MakeRequest(request, endpointResolutionOutcome.GetResult(), Aws::Http::HttpMethod::HTTP_GET, Aws::Auth::SIGV4_SIGNER));
}

GetStageDeploymentOutcomeCallable GameSparksClient::GetStageDeploymentCallable(const GetStageDeploymentRequest& request) const
{
  return MakeCallableOperation(ALLOCATION_TAG, &GameSparksClient::GetStageDeployment, this, request, m_executor.get());
}

void GameSparksClient::GetStageDeploymentAsync(const GetStageDeploymentRequest& request, const GetStageDeploymentResponseReceivedHandler& handler, const std::shared_ptr<const Aws::Client::AsyncCallerContext>& context) const
{
  MakeAsyncOperation(&GameSparksClient::GetStageDeployment, this, request, handler, context, m_executor.get());
}

ImportGameConfigurationOutcome GameSparksClient::ImportGameConfiguration(const ImportGameConfigurationRequest& request) const
{
  AWS_OPERATION_CHECK_PTR(m_endpointProvider, ImportGameConfiguration, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE);
  if (!request.GameNameHasBeenSet())
  {
    AWS_LOGSTREAM_ERROR("ImportGameConfiguration", "Required field: GameName, is not set");
    return ImportGameConfigurationOutcome(Aws::Client::AWSError<GameSparksErrors>(GameSparksErrors::MISSING_PARAMETER, "MISSING_PARAMETER", "Missing required field [GameName]", false));
  }
  ResolveEndpointOutcome endpointResolutionOutcome = m_endpointProvider->ResolveEndpoint(request.GetEndpointContextParams());
  AWS_OPERATION_CHECK_SUCCESS(endpointResolutionOutcome, ImportGameConfiguration, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE, endpointResolutionOutcome.GetError().GetMessage());
  endpointResolutionOutcome.GetResult().AddPathSegments("/game/");
  endpointResolutionOutcome.GetResult().AddPathSegment(request.GetGameName());
  endpointResolutionOutcome.GetResult().AddPathSegments("/configuration");
  return ImportGameConfigurationOutcome(MakeRequest(request, endpointResolutionOutcome.GetResult(), Aws::Http::HttpMethod::HTTP_PUT, Aws::Auth::SIGV4_SIGNER));
}

ImportGameConfigurationOutcomeCallable GameSparksClient::ImportGameConfigurationCallable(const ImportGameConfigurationRequest& request) const
{
  return MakeCallableOperation(ALLOCATION_TAG, &GameSparksClient::ImportGameConfiguration, this, request, m_executor.get());
}

void GameSparksClient::ImportGameConfigurationAsync(const ImportGameConfigurationRequest& request, const ImportGameConfigurationResponseReceivedHandler& handler, const std::shared_ptr<const Aws::Client::AsyncCallerContext>& context) const
{
  MakeAsyncOperation(&GameSparksClient::ImportGameConfiguration, this, request, handler, context, m_executor.get());
}

ListExtensionVersionsOutcome GameSparksClient::ListExtensionVersions(const ListExtensionVersionsRequest& request) const
{
  AWS_OPERATION_CHECK_PTR(m_endpointProvider, ListExtensionVersions, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE);
  if (!request.NameHasBeenSet())
  {
    AWS_LOGSTREAM_ERROR("ListExtensionVersions", "Required field: Name, is not set");
    return ListExtensionVersionsOutcome(Aws::Client::AWSError<GameSparksErrors>(GameSparksErrors::MISSING_PARAMETER, "MISSING_PARAMETER", "Missing required field [Name]", false));
  }
  if (!request.NamespaceHasBeenSet())
  {
    AWS_LOGSTREAM_ERROR("ListExtensionVersions", "Required field: Namespace, is not set");
    return ListExtensionVersionsOutcome(Aws::Client::AWSError<GameSparksErrors>(GameSparksErrors::MISSING_PARAMETER, "MISSING_PARAMETER", "Missing required field [Namespace]", false));
  }
  ResolveEndpointOutcome endpointResolutionOutcome = m_endpointProvider->ResolveEndpoint(request.GetEndpointContextParams());
  AWS_OPERATION_CHECK_SUCCESS(endpointResolutionOutcome, ListExtensionVersions, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE, endpointResolutionOutcome.GetError().GetMessage());
  endpointResolutionOutcome.GetResult().AddPathSegments("/extension/");
  endpointResolutionOutcome.GetResult().AddPathSegment(request.GetNamespace());
  endpointResolutionOutcome.GetResult().AddPathSegment(request.GetName());
  endpointResolutionOutcome.GetResult().AddPathSegments("/version");
  return ListExtensionVersionsOutcome(MakeRequest(request, endpointResolutionOutcome.GetResult(), Aws::Http::HttpMethod::HTTP_GET, Aws::Auth::SIGV4_SIGNER));
}

ListExtensionVersionsOutcomeCallable GameSparksClient::ListExtensionVersionsCallable(const ListExtensionVersionsRequest& request) const
{
  return MakeCallableOperation(ALLOCATION_TAG, &GameSparksClient::ListExtensionVersions, this, request, m_executor.get());
}

void GameSparksClient::ListExtensionVersionsAsync(const ListExtensionVersionsRequest& request, const ListExtensionVersionsResponseReceivedHandler& handler, const std::shared_ptr<const Aws::Client::AsyncCallerContext>& context) const
{
  MakeAsyncOperation(&GameSparksClient::ListExtensionVersions, this, request, handler, context, m_executor.get());
}

ListExtensionsOutcome GameSparksClient::ListExtensions(const ListExtensionsRequest& request) const
{
  AWS_OPERATION_CHECK_PTR(m_endpointProvider, ListExtensions, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE);
  ResolveEndpointOutcome endpointResolutionOutcome = m_endpointProvider->ResolveEndpoint(request.GetEndpointContextParams());
  AWS_OPERATION_CHECK_SUCCESS(endpointResolutionOutcome, ListExtensions, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE, endpointResolutionOutcome.GetError().GetMessage());
  endpointResolutionOutcome.GetResult().AddPathSegments("/extension");
  return ListExtensionsOutcome(MakeRequest(request, endpointResolutionOutcome.GetResult(), Aws::Http::HttpMethod::HTTP_GET, Aws::Auth::SIGV4_SIGNER));
}

ListExtensionsOutcomeCallable GameSparksClient::ListExtensionsCallable(const ListExtensionsRequest& request) const
{
  return MakeCallableOperation(ALLOCATION_TAG, &GameSparksClient::ListExtensions, this, request, m_executor.get());
}

void GameSparksClient::ListExtensionsAsync(const ListExtensionsRequest& request, const ListExtensionsResponseReceivedHandler& handler, const std::shared_ptr<const Aws::Client::AsyncCallerContext>& context) const
{
  MakeAsyncOperation(&GameSparksClient::ListExtensions, this, request, handler, context, m_executor.get());
}

ListGamesOutcome GameSparksClient::ListGames(const ListGamesRequest& request) const
{
  AWS_OPERATION_CHECK_PTR(m_endpointProvider, ListGames, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE);
  ResolveEndpointOutcome endpointResolutionOutcome = m_endpointProvider->ResolveEndpoint(request.GetEndpointContextParams());
  AWS_OPERATION_CHECK_SUCCESS(endpointResolutionOutcome, ListGames, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE, endpointResolutionOutcome.GetError().GetMessage());
  endpointResolutionOutcome.GetResult().AddPathSegments("/game");
  return ListGamesOutcome(MakeRequest(request, endpointResolutionOutcome.GetResult(), Aws::Http::HttpMethod::HTTP_GET, Aws::Auth::SIGV4_SIGNER));
}

ListGamesOutcomeCallable GameSparksClient::ListGamesCallable(const ListGamesRequest& request) const
{
  return MakeCallableOperation(ALLOCATION_TAG, &GameSparksClient::ListGames, this, request, m_executor.get());
}

void GameSparksClient::ListGamesAsync(const ListGamesRequest& request, const ListGamesResponseReceivedHandler& handler, const std::shared_ptr<const Aws::Client::AsyncCallerContext>& context) const
{
  MakeAsyncOperation(&GameSparksClient::ListGames, this, request, handler, context, m_executor.get());
}

ListGeneratedCodeJobsOutcome GameSparksClient::ListGeneratedCodeJobs(const ListGeneratedCodeJobsRequest& request) const
{
  AWS_OPERATION_CHECK_PTR(m_endpointProvider, ListGeneratedCodeJobs, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE);
  if (!request.GameNameHasBeenSet())
  {
    AWS_LOGSTREAM_ERROR("ListGeneratedCodeJobs", "Required field: GameName, is not set");
    return ListGeneratedCodeJobsOutcome(Aws::Client::AWSError<GameSparksErrors>(GameSparksErrors::MISSING_PARAMETER, "MISSING_PARAMETER", "Missing required field [GameName]", false));
  }
  if (!request.SnapshotIdHasBeenSet())
  {
    AWS_LOGSTREAM_ERROR("ListGeneratedCodeJobs", "Required field: SnapshotId, is not set");
    return ListGeneratedCodeJobsOutcome(Aws::Client::AWSError<GameSparksErrors>(GameSparksErrors::MISSING_PARAMETER, "MISSING_PARAMETER", "Missing required field [SnapshotId]", false));
  }
  ResolveEndpointOutcome endpointResolutionOutcome = m_endpointProvider->ResolveEndpoint(request.GetEndpointContextParams());
  AWS_OPERATION_CHECK_SUCCESS(endpointResolutionOutcome, ListGeneratedCodeJobs, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE, endpointResolutionOutcome.GetError().GetMessage());
  endpointResolutionOutcome.GetResult().AddPathSegments("/game/");
  endpointResolutionOutcome.GetResult().AddPathSegment(request.GetGameName());
  endpointResolutionOutcome.GetResult().AddPathSegments("/snapshot/");
  endpointResolutionOutcome.GetResult().AddPathSegment(request.GetSnapshotId());
  endpointResolutionOutcome.GetResult().AddPathSegments("/generated-sdk-code-jobs");
  return ListGeneratedCodeJobsOutcome(MakeRequest(request, endpointResolutionOutcome.GetResult(), Aws::Http::HttpMethod::HTTP_GET, Aws::Auth::SIGV4_SIGNER));
}

ListGeneratedCodeJobsOutcomeCallable GameSparksClient::ListGeneratedCodeJobsCallable(const ListGeneratedCodeJobsRequest& request) const
{
  return MakeCallableOperation(ALLOCATION_TAG, &GameSparksClient::ListGeneratedCodeJobs, this, request, m_executor.get());
}

void GameSparksClient::ListGeneratedCodeJobsAsync(const ListGeneratedCodeJobsRequest& request, const ListGeneratedCodeJobsResponseReceivedHandler& handler, const std::shared_ptr<const Aws::Client::AsyncCallerContext>& context) const
{
  MakeAsyncOperation(&GameSparksClient::ListGeneratedCodeJobs, this, request, handler, context, m_executor.get());
}

ListSnapshotsOutcome GameSparksClient::ListSnapshots(const ListSnapshotsRequest& request) const
{
  AWS_OPERATION_CHECK_PTR(m_endpointProvider, ListSnapshots, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE);
  if (!request.GameNameHasBeenSet())
  {
    AWS_LOGSTREAM_ERROR("ListSnapshots", "Required field: GameName, is not set");
    return ListSnapshotsOutcome(Aws::Client::AWSError<GameSparksErrors>(GameSparksErrors::MISSING_PARAMETER, "MISSING_PARAMETER", "Missing required field [GameName]", false));
  }
  ResolveEndpointOutcome endpointResolutionOutcome = m_endpointProvider->ResolveEndpoint(request.GetEndpointContextParams());
  AWS_OPERATION_CHECK_SUCCESS(endpointResolutionOutcome, ListSnapshots, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE, endpointResolutionOutcome.GetError().GetMessage());
  endpointResolutionOutcome.GetResult().AddPathSegments("/game/");
  endpointResolutionOutcome.GetResult().AddPathSegment(request.GetGameName());
  endpointResolutionOutcome.GetResult().AddPathSegments("/snapshot");
  return ListSnapshotsOutcome(MakeRequest(request, endpointResolutionOutcome.GetResult(), Aws::Http::HttpMethod::HTTP_GET, Aws::Auth::SIGV4_SIGNER));
}

ListSnapshotsOutcomeCallable GameSparksClient::ListSnapshotsCallable(const ListSnapshotsRequest& request) const
{
  return MakeCallableOperation(ALLOCATION_TAG, &GameSparksClient::ListSnapshots, this, request, m_executor.get());
}

void GameSparksClient::ListSnapshotsAsync(const ListSnapshotsRequest& request, const ListSnapshotsResponseReceivedHandler& handler, const std::shared_ptr<const Aws::Client::AsyncCallerContext>& context) const
{
  MakeAsyncOperation(&GameSparksClient::ListSnapshots, this, request, handler, context, m_executor.get());
}

ListStageDeploymentsOutcome GameSparksClient::ListStageDeployments(const ListStageDeploymentsRequest& request) const
{
  AWS_OPERATION_CHECK_PTR(m_endpointProvider, ListStageDeployments, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE);
  if (!request.GameNameHasBeenSet())
  {
    AWS_LOGSTREAM_ERROR("ListStageDeployments", "Required field: GameName, is not set");
    return ListStageDeploymentsOutcome(Aws::Client::AWSError<GameSparksErrors>(GameSparksErrors::MISSING_PARAMETER, "MISSING_PARAMETER", "Missing required field [GameName]", false));
  }
  if (!request.StageNameHasBeenSet())
  {
    AWS_LOGSTREAM_ERROR("ListStageDeployments", "Required field: StageName, is not set");
    return ListStageDeploymentsOutcome(Aws::Client::AWSError<GameSparksErrors>(GameSparksErrors::MISSING_PARAMETER, "MISSING_PARAMETER", "Missing required field [StageName]", false));
  }
  ResolveEndpointOutcome endpointResolutionOutcome = m_endpointProvider->ResolveEndpoint(request.GetEndpointContextParams());
  AWS_OPERATION_CHECK_SUCCESS(endpointResolutionOutcome, ListStageDeployments, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE, endpointResolutionOutcome.GetError().GetMessage());
  endpointResolutionOutcome.GetResult().AddPathSegments("/game/");
  endpointResolutionOutcome.GetResult().AddPathSegment(request.GetGameName());
  endpointResolutionOutcome.GetResult().AddPathSegments("/stage/");
  endpointResolutionOutcome.GetResult().AddPathSegment(request.GetStageName());
  endpointResolutionOutcome.GetResult().AddPathSegments("/deployments");
  return ListStageDeploymentsOutcome(MakeRequest(request, endpointResolutionOutcome.GetResult(), Aws::Http::HttpMethod::HTTP_GET, Aws::Auth::SIGV4_SIGNER));
}

ListStageDeploymentsOutcomeCallable GameSparksClient::ListStageDeploymentsCallable(const ListStageDeploymentsRequest& request) const
{
  return MakeCallableOperation(ALLOCATION_TAG, &GameSparksClient::ListStageDeployments, this, request, m_executor.get());
}

void GameSparksClient::ListStageDeploymentsAsync(const ListStageDeploymentsRequest& request, const ListStageDeploymentsResponseReceivedHandler& handler, const std::shared_ptr<const Aws::Client::AsyncCallerContext>& context) const
{
  MakeAsyncOperation(&GameSparksClient::ListStageDeployments, this, request, handler, context, m_executor.get());
}

ListStagesOutcome GameSparksClient::ListStages(const ListStagesRequest& request) const
{
  AWS_OPERATION_CHECK_PTR(m_endpointProvider, ListStages, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE);
  if (!request.GameNameHasBeenSet())
  {
    AWS_LOGSTREAM_ERROR("ListStages", "Required field: GameName, is not set");
    return ListStagesOutcome(Aws::Client::AWSError<GameSparksErrors>(GameSparksErrors::MISSING_PARAMETER, "MISSING_PARAMETER", "Missing required field [GameName]", false));
  }
  ResolveEndpointOutcome endpointResolutionOutcome = m_endpointProvider->ResolveEndpoint(request.GetEndpointContextParams());
  AWS_OPERATION_CHECK_SUCCESS(endpointResolutionOutcome, ListStages, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE, endpointResolutionOutcome.GetError().GetMessage());
  endpointResolutionOutcome.GetResult().AddPathSegments("/game/");
  endpointResolutionOutcome.GetResult().AddPathSegment(request.GetGameName());
  endpointResolutionOutcome.GetResult().AddPathSegments("/stage");
  return ListStagesOutcome(MakeRequest(request, endpointResolutionOutcome.GetResult(), Aws::Http::HttpMethod::HTTP_GET, Aws::Auth::SIGV4_SIGNER));
}

ListStagesOutcomeCallable GameSparksClient::ListStagesCallable(const ListStagesRequest& request) const
{
  return MakeCallableOperation(ALLOCATION_TAG, &GameSparksClient::ListStages, this, request, m_executor.get());
}

void GameSparksClient::ListStagesAsync(const ListStagesRequest& request, const ListStagesResponseReceivedHandler& handler, const std::shared_ptr<const Aws::Client::AsyncCallerContext>& context) const
{
  MakeAsyncOperation(&GameSparksClient::ListStages, this, request, handler, context, m_executor.get());
}

ListTagsForResourceOutcome GameSparksClient::ListTagsForResource(const ListTagsForResourceRequest& request) const
{
  AWS_OPERATION_CHECK_PTR(m_endpointProvider, ListTagsForResource, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE);
  if (!request.ResourceArnHasBeenSet())
  {
    AWS_LOGSTREAM_ERROR("ListTagsForResource", "Required field: ResourceArn, is not set");
    return ListTagsForResourceOutcome(Aws::Client::AWSError<GameSparksErrors>(GameSparksErrors::MISSING_PARAMETER, "MISSING_PARAMETER", "Missing required field [ResourceArn]", false));
  }
  ResolveEndpointOutcome endpointResolutionOutcome = m_endpointProvider->ResolveEndpoint(request.GetEndpointContextParams());
  AWS_OPERATION_CHECK_SUCCESS(endpointResolutionOutcome, ListTagsForResource, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE, endpointResolutionOutcome.GetError().GetMessage());
  endpointResolutionOutcome.GetResult().AddPathSegments("/tags/");
  endpointResolutionOutcome.GetResult().AddPathSegment(request.GetResourceArn());
  return ListTagsForResourceOutcome(MakeRequest(request, endpointResolutionOutcome.GetResult(), Aws::Http::HttpMethod::HTTP_GET, Aws::Auth::SIGV4_SIGNER));
}

ListTagsForResourceOutcomeCallable GameSparksClient::ListTagsForResourceCallable(const ListTagsForResourceRequest& request) const
{
  return MakeCallableOperation(ALLOCATION_TAG, &GameSparksClient::ListTagsForResource, this, request, m_executor.get());
}

void GameSparksClient::ListTagsForResourceAsync(const ListTagsForResourceRequest& request, const ListTagsForResourceResponseReceivedHandler& handler, const std::shared_ptr<const Aws::Client::AsyncCallerContext>& context) const
{
  MakeAsyncOperation(&GameSparksClient::ListTagsForResource, this, request, handler, context, m_executor.get());
}

StartGeneratedCodeJobOutcome GameSparksClient::StartGeneratedCodeJob(const StartGeneratedCodeJobRequest& request) const
{
  AWS_OPERATION_CHECK_PTR(m_endpointProvider, StartGeneratedCodeJob, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE);
  if (!request.GameNameHasBeenSet())
  {
    AWS_LOGSTREAM_ERROR("StartGeneratedCodeJob", "Required field: GameName, is not set");
    return StartGeneratedCodeJobOutcome(Aws::Client::AWSError<GameSparksErrors>(GameSparksErrors::MISSING_PARAMETER, "MISSING_PARAMETER", "Missing required field [GameName]", false));
  }
  if (!request.SnapshotIdHasBeenSet())
  {
    AWS_LOGSTREAM_ERROR("StartGeneratedCodeJob", "Required field: SnapshotId, is not set");
    return StartGeneratedCodeJobOutcome(Aws::Client::AWSError<GameSparksErrors>(GameSparksErrors::MISSING_PARAMETER, "MISSING_PARAMETER", "Missing required field [SnapshotId]", false));
  }
  ResolveEndpointOutcome endpointResolutionOutcome = m_endpointProvider->ResolveEndpoint(request.GetEndpointContextParams());
  AWS_OPERATION_CHECK_SUCCESS(endpointResolutionOutcome, StartGeneratedCodeJob, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE, endpointResolutionOutcome.GetError().GetMessage());
  endpointResolutionOutcome.GetResult().AddPathSegments("/game/");
  endpointResolutionOutcome.GetResult().AddPathSegment(request.GetGameName());
  endpointResolutionOutcome.GetResult().AddPathSegments("/snapshot/");
  endpointResolutionOutcome.GetResult().AddPathSegment(request.GetSnapshotId());
  endpointResolutionOutcome.GetResult().AddPathSegments("/generated-sdk-code-job");
  return StartGeneratedCodeJobOutcome(MakeRequest(request, endpointResolutionOutcome.GetResult(), Aws::Http::HttpMethod::HTTP_POST, Aws::Auth::SIGV4_SIGNER));
}

StartGeneratedCodeJobOutcomeCallable GameSparksClient::StartGeneratedCodeJobCallable(const StartGeneratedCodeJobRequest& request) const
{
  return MakeCallableOperation(ALLOCATION_TAG, &GameSparksClient::StartGeneratedCodeJob, this, request, m_executor.get());
}

void GameSparksClient::StartGeneratedCodeJobAsync(const StartGeneratedCodeJobRequest& request, const StartGeneratedCodeJobResponseReceivedHandler& handler, const std::shared_ptr<const Aws::Client::AsyncCallerContext>& context) const
{
  MakeAsyncOperation(&GameSparksClient::StartGeneratedCodeJob, this, request, handler, context, m_executor.get());
}

StartStageDeploymentOutcome GameSparksClient::StartStageDeployment(const StartStageDeploymentRequest& request) const
{
  AWS_OPERATION_CHECK_PTR(m_endpointProvider, StartStageDeployment, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE);
  if (!request.GameNameHasBeenSet())
  {
    AWS_LOGSTREAM_ERROR("StartStageDeployment", "Required field: GameName, is not set");
    return StartStageDeploymentOutcome(Aws::Client::AWSError<GameSparksErrors>(GameSparksErrors::MISSING_PARAMETER, "MISSING_PARAMETER", "Missing required field [GameName]", false));
  }
  if (!request.StageNameHasBeenSet())
  {
    AWS_LOGSTREAM_ERROR("StartStageDeployment", "Required field: StageName, is not set");
    return StartStageDeploymentOutcome(Aws::Client::AWSError<GameSparksErrors>(GameSparksErrors::MISSING_PARAMETER, "MISSING_PARAMETER", "Missing required field [StageName]", false));
  }
  ResolveEndpointOutcome endpointResolutionOutcome = m_endpointProvider->ResolveEndpoint(request.GetEndpointContextParams());
  AWS_OPERATION_CHECK_SUCCESS(endpointResolutionOutcome, StartStageDeployment, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE, endpointResolutionOutcome.GetError().GetMessage());
  endpointResolutionOutcome.GetResult().AddPathSegments("/game/");
  endpointResolutionOutcome.GetResult().AddPathSegment(request.GetGameName());
  endpointResolutionOutcome.GetResult().AddPathSegments("/stage/");
  endpointResolutionOutcome.GetResult().AddPathSegment(request.GetStageName());
  endpointResolutionOutcome.GetResult().AddPathSegments("/deployment");
  return StartStageDeploymentOutcome(MakeRequest(request, endpointResolutionOutcome.GetResult(), Aws::Http::HttpMethod::HTTP_POST, Aws::Auth::SIGV4_SIGNER));
}

StartStageDeploymentOutcomeCallable GameSparksClient::StartStageDeploymentCallable(const StartStageDeploymentRequest& request) const
{
  return MakeCallableOperation(ALLOCATION_TAG, &GameSparksClient::StartStageDeployment, this, request, m_executor.get());
}

void GameSparksClient::StartStageDeploymentAsync(const StartStageDeploymentRequest& request, const StartStageDeploymentResponseReceivedHandler& handler, const std::shared_ptr<const Aws::Client::AsyncCallerContext>& context) const
{
  MakeAsyncOperation(&GameSparksClient::StartStageDeployment, this, request, handler, context, m_executor.get());
}

TagResourceOutcome GameSparksClient::TagResource(const TagResourceRequest& request) const
{
  AWS_OPERATION_CHECK_PTR(m_endpointProvider, TagResource, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE);
  if (!request.ResourceArnHasBeenSet())
  {
    AWS_LOGSTREAM_ERROR("TagResource", "Required field: ResourceArn, is not set");
    return TagResourceOutcome(Aws::Client::AWSError<GameSparksErrors>(GameSparksErrors::MISSING_PARAMETER, "MISSING_PARAMETER", "Missing required field [ResourceArn]", false));
  }
  ResolveEndpointOutcome endpointResolutionOutcome = m_endpointProvider->ResolveEndpoint(request.GetEndpointContextParams());
  AWS_OPERATION_CHECK_SUCCESS(endpointResolutionOutcome, TagResource, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE, endpointResolutionOutcome.GetError().GetMessage());
  endpointResolutionOutcome.GetResult().AddPathSegments("/tags/");
  endpointResolutionOutcome.GetResult().AddPathSegment(request.GetResourceArn());
  return TagResourceOutcome(MakeRequest(request, endpointResolutionOutcome.GetResult(), Aws::Http::HttpMethod::HTTP_POST, Aws::Auth::SIGV4_SIGNER));
}

TagResourceOutcomeCallable GameSparksClient::TagResourceCallable(const TagResourceRequest& request) const
{
  return MakeCallableOperation(ALLOCATION_TAG, &GameSparksClient::TagResource, this, request, m_executor.get());
}

void GameSparksClient::TagResourceAsync(const TagResourceRequest& request, const TagResourceResponseReceivedHandler& handler, const std::shared_ptr<const Aws::Client::AsyncCallerContext>& context) const
{
  MakeAsyncOperation(&GameSparksClient::TagResource, this, request, handler, context, m_executor.get());
}

UntagResourceOutcome GameSparksClient::UntagResource(const UntagResourceRequest& request) const
{
  AWS_OPERATION_CHECK_PTR(m_endpointProvider, UntagResource, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE);
  if (!request.ResourceArnHasBeenSet())
  {
    AWS_LOGSTREAM_ERROR("UntagResource", "Required field: ResourceArn, is not set");
    return UntagResourceOutcome(Aws::Client::AWSError<GameSparksErrors>(GameSparksErrors::MISSING_PARAMETER, "MISSING_PARAMETER", "Missing required field [ResourceArn]", false));
  }
  if (!request.TagKeysHasBeenSet())
  {
    AWS_LOGSTREAM_ERROR("UntagResource", "Required field: TagKeys, is not set");
    return UntagResourceOutcome(Aws::Client::AWSError<GameSparksErrors>(GameSparksErrors::MISSING_PARAMETER, "MISSING_PARAMETER", "Missing required field [TagKeys]", false));
  }
  ResolveEndpointOutcome endpointResolutionOutcome = m_endpointProvider->ResolveEndpoint(request.GetEndpointContextParams());
  AWS_OPERATION_CHECK_SUCCESS(endpointResolutionOutcome, UntagResource, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE, endpointResolutionOutcome.GetError().GetMessage());
  endpointResolutionOutcome.GetResult().AddPathSegments("/tags/");
  endpointResolutionOutcome.GetResult().AddPathSegment(request.GetResourceArn());
  return UntagResourceOutcome(MakeRequest(request, endpointResolutionOutcome.GetResult(), Aws::Http::HttpMethod::HTTP_DELETE, Aws::Auth::SIGV4_SIGNER));
}

UntagResourceOutcomeCallable GameSparksClient::UntagResourceCallable(const UntagResourceRequest& request) const
{
  return MakeCallableOperation(ALLOCATION_TAG, &GameSparksClient::UntagResource, this, request, m_executor.get());
}

void GameSparksClient::UntagResourceAsync(const UntagResourceRequest& request, const UntagResourceResponseReceivedHandler& handler, const std::shared_ptr<const Aws::Client::AsyncCallerContext>& context) const
{
  MakeAsyncOperation(&GameSparksClient::UntagResource, this, request, handler, context, m_executor.get());
}

UpdateGameOutcome GameSparksClient::UpdateGame(const UpdateGameRequest& request) const
{
  AWS_OPERATION_CHECK_PTR(m_endpointProvider, UpdateGame, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE);
  if (!request.GameNameHasBeenSet())
  {
    AWS_LOGSTREAM_ERROR("UpdateGame", "Required field: GameName, is not set");
    return UpdateGameOutcome(Aws::Client::AWSError<GameSparksErrors>(GameSparksErrors::MISSING_PARAMETER, "MISSING_PARAMETER", "Missing required field [GameName]", false));
  }
  ResolveEndpointOutcome endpointResolutionOutcome = m_endpointProvider->ResolveEndpoint(request.GetEndpointContextParams());
  AWS_OPERATION_CHECK_SUCCESS(endpointResolutionOutcome, UpdateGame, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE, endpointResolutionOutcome.GetError().GetMessage());
  endpointResolutionOutcome.GetResult().AddPathSegments("/game/");
  endpointResolutionOutcome.GetResult().AddPathSegment(request.GetGameName());
  return UpdateGameOutcome(MakeRequest(request, endpointResolutionOutcome.GetResult(), Aws::Http::HttpMethod::HTTP_PATCH, Aws::Auth::SIGV4_SIGNER));
}

UpdateGameOutcomeCallable GameSparksClient::UpdateGameCallable(const UpdateGameRequest& request) const
{
  return MakeCallableOperation(ALLOCATION_TAG, &GameSparksClient::UpdateGame, this, request, m_executor.get());
}

void GameSparksClient::UpdateGameAsync(const UpdateGameRequest& request, const UpdateGameResponseReceivedHandler& handler, const std::shared_ptr<const Aws::Client::AsyncCallerContext>& context) const
{
  MakeAsyncOperation(&GameSparksClient::UpdateGame, this, request, handler, context, m_executor.get());
}

UpdateGameConfigurationOutcome GameSparksClient::UpdateGameConfiguration(const UpdateGameConfigurationRequest& request) const
{
  AWS_OPERATION_CHECK_PTR(m_endpointProvider, UpdateGameConfiguration, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE);
  if (!request.GameNameHasBeenSet())
  {
    AWS_LOGSTREAM_ERROR("UpdateGameConfiguration", "Required field: GameName, is not set");
    return UpdateGameConfigurationOutcome(Aws::Client::AWSError<GameSparksErrors>(GameSparksErrors::MISSING_PARAMETER, "MISSING_PARAMETER", "Missing required field [GameName]", false));
  }
  ResolveEndpointOutcome endpointResolutionOutcome = m_endpointProvider->ResolveEndpoint(request.GetEndpointContextParams());
  AWS_OPERATION_CHECK_SUCCESS(endpointResolutionOutcome, UpdateGameConfiguration, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE, endpointResolutionOutcome.GetError().GetMessage());
  endpointResolutionOutcome.GetResult().AddPathSegments("/game/");
  endpointResolutionOutcome.GetResult().AddPathSegment(request.GetGameName());
  endpointResolutionOutcome.GetResult().AddPathSegments("/configuration");
  return UpdateGameConfigurationOutcome(MakeRequest(request, endpointResolutionOutcome.GetResult(), Aws::Http::HttpMethod::HTTP_PATCH, Aws::Auth::SIGV4_SIGNER));
}

UpdateGameConfigurationOutcomeCallable GameSparksClient::UpdateGameConfigurationCallable(const UpdateGameConfigurationRequest& request) const
{
  return MakeCallableOperation(ALLOCATION_TAG, &GameSparksClient::UpdateGameConfiguration, this, request, m_executor.get());
}

void GameSparksClient::UpdateGameConfigurationAsync(const UpdateGameConfigurationRequest& request, const UpdateGameConfigurationResponseReceivedHandler& handler, const std::shared_ptr<const Aws::Client::AsyncCallerContext>& context) const
{
  MakeAsyncOperation(&GameSparksClient::UpdateGameConfiguration, this, request, handler, context, m_executor.get());
}

UpdateSnapshotOutcome GameSparksClient::UpdateSnapshot(const UpdateSnapshotRequest& request) const
{
  AWS_OPERATION_CHECK_PTR(m_endpointProvider, UpdateSnapshot, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE);
  if (!request.GameNameHasBeenSet())
  {
    AWS_LOGSTREAM_ERROR("UpdateSnapshot", "Required field: GameName, is not set");
    return UpdateSnapshotOutcome(Aws::Client::AWSError<GameSparksErrors>(GameSparksErrors::MISSING_PARAMETER, "MISSING_PARAMETER", "Missing required field [GameName]", false));
  }
  if (!request.SnapshotIdHasBeenSet())
  {
    AWS_LOGSTREAM_ERROR("UpdateSnapshot", "Required field: SnapshotId, is not set");
    return UpdateSnapshotOutcome(Aws::Client::AWSError<GameSparksErrors>(GameSparksErrors::MISSING_PARAMETER, "MISSING_PARAMETER", "Missing required field [SnapshotId]", false));
  }
  ResolveEndpointOutcome endpointResolutionOutcome = m_endpointProvider->ResolveEndpoint(request.GetEndpointContextParams());
  AWS_OPERATION_CHECK_SUCCESS(endpointResolutionOutcome, UpdateSnapshot, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE, endpointResolutionOutcome.GetError().GetMessage());
  endpointResolutionOutcome.GetResult().AddPathSegments("/game/");
  endpointResolutionOutcome.GetResult().AddPathSegment(request.GetGameName());
  endpointResolutionOutcome.GetResult().AddPathSegments("/snapshot/");
  endpointResolutionOutcome.GetResult().AddPathSegment(request.GetSnapshotId());
  return UpdateSnapshotOutcome(MakeRequest(request, endpointResolutionOutcome.GetResult(), Aws::Http::HttpMethod::HTTP_PATCH, Aws::Auth::SIGV4_SIGNER));
}

UpdateSnapshotOutcomeCallable GameSparksClient::UpdateSnapshotCallable(const UpdateSnapshotRequest& request) const
{
  return MakeCallableOperation(ALLOCATION_TAG, &GameSparksClient::UpdateSnapshot, this, request, m_executor.get());
}

void GameSparksClient::UpdateSnapshotAsync(const UpdateSnapshotRequest& request, const UpdateSnapshotResponseReceivedHandler& handler, const std::shared_ptr<const Aws::Client::AsyncCallerContext>& context) const
{
  MakeAsyncOperation(&GameSparksClient::UpdateSnapshot, this, request, handler, context, m_executor.get());
}

UpdateStageOutcome GameSparksClient::UpdateStage(const UpdateStageRequest& request) const
{
  AWS_OPERATION_CHECK_PTR(m_endpointProvider, UpdateStage, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE);
  if (!request.GameNameHasBeenSet())
  {
    AWS_LOGSTREAM_ERROR("UpdateStage", "Required field: GameName, is not set");
    return UpdateStageOutcome(Aws::Client::AWSError<GameSparksErrors>(GameSparksErrors::MISSING_PARAMETER, "MISSING_PARAMETER", "Missing required field [GameName]", false));
  }
  if (!request.StageNameHasBeenSet())
  {
    AWS_LOGSTREAM_ERROR("UpdateStage", "Required field: StageName, is not set");
    return UpdateStageOutcome(Aws::Client::AWSError<GameSparksErrors>(GameSparksErrors::MISSING_PARAMETER, "MISSING_PARAMETER", "Missing required field [StageName]", false));
  }
  ResolveEndpointOutcome endpointResolutionOutcome = m_endpointProvider->ResolveEndpoint(request.GetEndpointContextParams());
  AWS_OPERATION_CHECK_SUCCESS(endpointResolutionOutcome, UpdateStage, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE, endpointResolutionOutcome.GetError().GetMessage());
  endpointResolutionOutcome.GetResult().AddPathSegments("/game/");
  endpointResolutionOutcome.GetResult().AddPathSegment(request.GetGameName());
  endpointResolutionOutcome.GetResult().AddPathSegments("/stage/");
  endpointResolutionOutcome.GetResult().AddPathSegment(request.GetStageName());
  return UpdateStageOutcome(MakeRequest(request, endpointResolutionOutcome.GetResult(), Aws::Http::HttpMethod::HTTP_PATCH, Aws::Auth::SIGV4_SIGNER));
}

UpdateStageOutcomeCallable GameSparksClient::UpdateStageCallable(const UpdateStageRequest& request) const
{
  return MakeCallableOperation(ALLOCATION_TAG, &GameSparksClient::UpdateStage, this, request, m_executor.get());
}

void GameSparksClient::UpdateStageAsync(const UpdateStageRequest& request, const UpdateStageResponseReceivedHandler& handler, const std::shared_ptr<const Aws::Client::AsyncCallerContext>& context) const
{
  MakeAsyncOperation(&GameSparksClient::UpdateStage, this, request, handler, context, m_executor.get());
}

