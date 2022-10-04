export interface Protocol {
  code: number
  size: number
  name: string
  resolvable?: boolean | undefined
  path?: boolean | undefined
}

export interface MultiaddrObject {
  family: 4 | 6
  host: string
  transport: string
  port: number
}
