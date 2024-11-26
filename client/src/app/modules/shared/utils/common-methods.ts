import { shared } from "../shared";

export function getRandomString(length:number = shared.definitions.ID_DIGITS_COUNT): string {
  const signs = 'ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789';
  let out = '';
  for (let i = 0; i < length; i++) {
    // Dla przykładu generujemy 10-znakowy ciąg
    out += signs.charAt(Math.floor(Math.random() * signs.length));
  }
  
  return out;
}

export function getRandomNumber(min: number = shared.definitions.ID_MIN_NUMBER, max: number = shared.definitions.ID_MAX_NUMBER): number {
  return Math.floor(Math.random() * (max - min + 1) + min);
}

