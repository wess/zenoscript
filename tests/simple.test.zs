import { test, expect } from "bun:test"

test("simple zenoscript test", () => {
  let x = 42
  expect(x).toBe(42)
})

test("zenoscript pipe operator", () => {
  let result = 5 |> (x => x * 2) |> (x => x + 1)
  expect(result).toBe(11)
})