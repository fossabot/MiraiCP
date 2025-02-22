/*
 * Copyright (c) 2020 - 2021. Eritque arcus and contributors.
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Affero General Public License as
 * published by the Free Software Foundation, either version 3 of the
 * License, or any later version(in your opinion).
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU Affero General Public License for more details.
 *
 * You should have received a copy of the GNU Affero General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 *
 */

package tech.eritquearcus.miraicp.loader

import group.GroupMessageTest
import org.junit.jupiter.api.Test
import java.io.ByteArrayOutputStream
import java.io.PrintStream

internal val outContent = ByteArrayOutputStream()
internal val errContent = ByteArrayOutputStream()
internal val originOut = System.out
private val originErr = System.err

internal fun setUpStreams() {
    System.setOut(PrintStream(outContent))
    System.setErr(PrintStream(errContent))
}

internal fun restoreStreams() {
    System.setOut(originOut)
    System.setOut(originErr)
}

class Test {

    @Test
    fun groupTest() {
        GroupMessageTest().lotGroupMessageTest()
    }
}