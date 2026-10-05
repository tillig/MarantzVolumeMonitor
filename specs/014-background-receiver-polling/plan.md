# Implementation Plan: Background Receiver Polling

**Branch**: `feature/014-background-receiver-polling` | **Date**: 2026-10-02 | **Spec**: [spec.md](spec.md)

## Summary

`HomeScreen` fetched status synchronously from its update path, so an unreachable receiver blocked the UI loop for the full HTTP timeout and dropped taps. A new `ReceiverMonitor` fetches on a FreeRTOS task and caches the latest result; screens read the cache.

## Technical Context

**Language/Version**: C++/Arduino on pioarduino `platform-espressif32` (Arduino-ESP32 3.x)

**Primary Dependencies**: Built-in `HTTPClient` through `MarantzClient`; FreeRTOS task notifications.

**Testing**: Hardware verification per [tasks.md](tasks.md).

## Constitution Check

- **I. Visibility-First UI**: A new `Connecting to receiver` state replaces the misleading `Connecting Wi-Fi` while the first request runs, and stale results show as unavailable.
- **II. Passive Appliance Stability**: Status requests can no longer stall touch handling on Home.
- **III. Strict Layered Architecture**: Polling lives in the network layer; screens only read the cache.

## Design Notes

- The cache is mutex-protected because the poller task and UI loop share it.
- Each read records its time. The task idles when no screen has read status for a few seconds, and the next read wakes it.
- A sequence number changes with every result, so `HomeScreen` refreshes as soon as one arrives.
- Changing the receiver address clears the cache and wakes the task; results for the old address are dropped.
